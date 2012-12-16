;; See https://github.com/overtone/overtone/blob/master/src/overtone/jna_path.clj
;; This should let us read straight from jars
(defonce __SET_JNA_PATH__
  (do
  (println (System/getProperty "jna.library.path"))
  (println (System/getProperty "java.library.path"))
  (System/setProperty "jna.library.path" (System/getProperty "java.library.path"))))

(ns v8.core
  (:require [clojure.string :as str])
  (:import [com.sun.jna WString Native Memory Pointer NativeLibrary]))

;; On linux, libraries are properly loaded using java.library.path/jna.library.path. However, that's only for libraries loaded directly from JNA. Libraries loaded by those libraries do not use java.library.path, only LD_LIBRARY_PATH, which is only set at the very start of the process and cannot be set in code. The solution is to load libv8.clj-v8 oursevles so that it is already loaded when v8wrapper is loaded.
;; In addition, while JNA allows you to specify the full name of a lib (that is, containing the "lib" and the version), it only allows this when the version number is all digits and dots. We can work around it with an absolute path.
(when (com.sun.jna.Platform/isLinux)
  (doseq [path (str/split (System/getProperty "jna.library.path") #":")]
    (let [abs (-> (str path "/" "libv8.so.clj-v8") java.io.File. .getAbsolutePath)]
      (com.sun.jna.NativeLibrary/getInstance abs))))

(def LIBRARY (com.sun.jna.NativeLibrary/getInstance "v8wrapper"))

(def run-fn (.getFunction LIBRARY "run"))
(def create-tuple-fn (.getFunction LIBRARY "create_tuple"))
(def cleanup-tuple-fn (.getFunction LIBRARY "cleanup_tuple"))

(defn create-context
  "Creates a V8 context and associated structures"
  []
  (.invokePointer create-tuple-fn (into-array [])))

(defn run-script-in-context
  "Compile and run a JS script within the given context"
  [cx script]
  (let [result (.invoke run-fn Memory (object-array [cx (new WString script)]))
        strresult (if (nil? result) nil (.getString result 0 true))]
    (when (not= (. Native getLastError) 0)
      (if (nil? result)
        (throw (Exception. "V8 reported error, but message is null!"))
        (throw (Exception. (str "V8 error: " strresult)))))
    strresult))

(defn cleanup-context
  "Cleans the memory from a context"
  [cx]
  (.invokeVoid cleanup-tuple-fn (into-array [cx])))

(defn run-script
  "Compiles and runs a JS file"
  [script]
  (let [cx (create-context)]
    (try
      (run-script-in-context cx script)
      (finally
       (cleanup-context cx)))))
