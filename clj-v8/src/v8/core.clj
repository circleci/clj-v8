(ns v8.core
  (:require [net.n01se.clojure-jna :as jna])
  (:import [com.sun.jna Pointer WString]))

(defn run-script
  "Compiles and runs a JS file"
  [script]
  (.toString ((jna/jna-fn WString v8wrapper/run) script)))
