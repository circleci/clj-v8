(ns v8.core
  (:require [net.n01se.clojure-jna :as jna])
  (:import [com.sun.jna WString Native Memory Pointer NativeLibrary]))

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
