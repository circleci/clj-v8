(ns v8.core
  (:require [clojure.java.io :as io])
  (:import [com.sun.jna Memory Native NativeLibrary Pointer WString]
           [java.io File FileOutputStream]))

(def libraries ["icui18n" "icuuc" "v8" "v8wrapper"])

(def library-path-fragments
  (let [os-name (System/getProperty "os.name")
        os-arch (System/getProperty "os.arch")]
    (case [os-name os-arch]
      ["Mac OS X" "x86_64"] ["macosx-x86-64/" ".dylib"]
      ["Linux" "x86_64"]    ["linux-x86-64/" ".so"]
      ["Linux" "amd64"]     ["linux-x86-64/" ".so"]
      ["Linux" "x86"]       ["linux-x86/" ".so"]
      ["Linux" "i386"]      ["linux-x86/" ".so"]
      ["Linux" "i486"]      ["linux-x86/" ".so"]
      ["Linux" "i586"]      ["linux-x86/" ".so"]
      ["Linux" "i686"]      ["linux-x86/" ".so"]
      (throw (Exception. (str "Unsupported system: " os-name " " os-arch))))))

(def library-path
  (let [[parent-dir ext] library-path-fragments
        user-dir (System/getProperty "user.dir")
        native-dir (io/as-file (str user-dir "/" "target/native"))]
    (if (.isDirectory native-dir)
      ;; extracted automatically by leiningen
      (.getAbsolutePath (File. native-dir parent-dir))
      ;; must extract manually
      (let [tmp-dir (File. (System/getProperty "java.io.tmpdir")
                           (str "/clj-v8/" parent-dir))]
        (.mkdirs tmp-dir)
        (doall (->> libraries
                    (map #(str % ext)) ; append extension
                    (map #(let [in (-> (str "native/" parent-dir %)
                                       io/resource
                                       io/as-file)
                                out (File. tmp-dir %)]
                            (io/copy in out)))))
        (.getAbsolutePath tmp-dir)))))

;; tell JNA where to find the libraries
(System/setProperty "jna.library.path" library-path)
  
(def LIBRARY (NativeLibrary/getInstance "v8wrapper"))

(def cleanup-tuple-fn (.getFunction LIBRARY "CleanupTuple"))
(def create-tuple-fn (.getFunction LIBRARY "CreateTuple"))
(def initialize-fn (.getFunction LIBRARY "Initialize"))
(def initialize-icu-fn (.getFunction LIBRARY "InitializeICU"))
(def run-fn (.getFunction LIBRARY "Run"))
(def set-flags-fn (.getFunction LIBRARY "SetFlags"))

(defn initialize
  "Explicitly initialize V8. Note that this is done implicitly as soon as any
  part of the V8 API is accessed."
  []
  (.invokeVoid initialize-fn (into-array [])))

(defn initialize-icu
  "Initialize I18N (ECMAScript 402) in V8."
  []
  (.invokeVoid initialize-icu-fn (into-array [])))

(defn set-flags
  "Set V8 flags. Must be called before V8 is initialized. See flag-definitions.h
  for the full list of flags available: http://git.io/HPcDiw"
  [& flags]
  (let [args (object-array [(->> flags
                                 (interpose " ")
                                 (apply str)
                                 (WString.))])]
    (.invokeVoid set-flags-fn args)))

(defn create-context
  "Creates a V8 context and associated structures"
  []
  (.invokePointer create-tuple-fn (into-array [])))

(defn run-script-in-context
  "Compile and run JavaScript code within the given context"
  ([cx source]
     (run-script-in-context cx source "unknown"))
  ([cx source name]
     (let [args [cx (WString. source) (WString. name)]
           result (.invoke run-fn Memory (object-array args))
           result (when result
                    (.getString result 0 true))]
       (when (not= (. Native getLastError) 0)
         (if (nil? result)
           (throw (Exception. "V8 reported error, but message is null!"))
           (throw (Exception. (str "V8 error: " result)))))
       result)))

(defn cleanup-context
  "Cleans the memory from a context"
  [cx]
  (.invokeVoid cleanup-tuple-fn (into-array [cx])))

(defn run-script
  "Compiles and runs a JS file"
  [source]
  (let [cx (create-context)]
    (try
      (run-script-in-context cx source)
      (finally
        (cleanup-context cx)))))
