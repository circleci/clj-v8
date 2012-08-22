(ns v8.core
  (:require [net.n01se.clojure-jna :as jna])
  (:import [com.sun.jna WString Native Memory NativeLibrary]))

(def LIBRARY (com.sun.jna.NativeLibrary/getInstance "v8wrapper"))

(def library-run (.getFunction LIBRARY "run"))
(def library-cleanup (.getFunction LIBRARY "cleanup"))

(def v8LockingObject (Object.))

(defn run-script
  "Compiles and runs a JS file"
  [script]
  (locking v8LockingObject
    (let [result (.invoke library-run Memory (into-array [(new WString script)]))]
      (if (== (. Native getLastError) 0)
        (if (nil? result)
          nil
          (let [strresult (.getString result 0 true)]
            (comment (.invokeVoid library-cleanup (into-array [result])))
            strresult))

        (if (nil? result)
          (throw (Exception. "V8 reported error, but message is null!"))
          
          (let [exres (Exception. (str "V8 error: " (.getString result 0 true)))]
            (comment (.invokeVoid library-cleanup (into-array [result])))
            (throw exres)))))))
