(ns v8.core
  (:require [net.n01se.clojure-jna :as jna])
  (:import [com.sun.jna Pointer WString]))

(defn run-script
  "Compiles and runs a JS file"
  [script]
  (let [result ((jna/jna-fn WString v8wrapper/run) (new WString script))]
       (if (nil? result) nil (.toString result))))

