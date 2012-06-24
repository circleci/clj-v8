(ns v8.core
  (:require [net.n01se.clojure-jna :as jna])
  (:import [com.sun.jna WString Native]))

(defn run-script
  "Compiles and runs a JS file"
  [script]
  (let [result ((jna/jna-fn WString v8wrapper/run) (new WString script))]
    (if (== (. Native getLastError) 0)
      (if (nil? result) nil (.toString result))
      (throw (Exception. (if (nil? result)
                           "V8 returned null as error message"
                           (str "V8 error: " (.toString result))))))))
