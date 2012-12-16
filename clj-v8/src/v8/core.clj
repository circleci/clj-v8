(ns v8.core
  (:require [net.n01se.clojure-jna :as jna])
  (:import [com.sun.jna WString Native Memory Pointer NativeLibrary]))

(def LIBRARY (com.sun.jna.NativeLibrary/getInstance "v8wrapper"))

(def run-fn (.getFunction LIBRARY "run"))
(def create-tuple-fn (.getFunction LIBRARY "create_tuple"))
(def cleanup-tuple-fn (.getFunction LIBRARY "cleanup_tuple"))


(def v8LockingObject (Object.))

(defn run-script
  "Compiles and runs a JS file"
  [script]
  (locking v8LockingObject
    (let [tuple (.invokePointer create-tuple-fn (into-array []))
          result (.invoke run-fn Memory (object-array [tuple (new WString script)]))
          strresult (if (nil? result) nil (.getString result 0 true))]

      ;; result will be cleaned up by the finalizer, hopefully
      (when (not= (. Native getLastError) 0)
        (.invokeVoid cleanup-tuple-fn (into-array [tuple]))
        (if (nil? result)
          (throw (Exception. "V8 reported error, but message is null!"))
          (throw (Exception. (str "V8 error: " strresult)))))
      (.invokeVoid cleanup-tuple-fn (into-array [tuple]))
      strresult)))
