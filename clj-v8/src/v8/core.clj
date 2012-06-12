(ns v8.core
  (:require [net.n01se.clojure-jna :as jna])
  (:import [com.sun.jna Pointer]))

(defn run-script
  "Compiles and runs a JS file"
  [script]
  (System/setProperty "jna.encoding" "UTF8")
  ((jna/jna-fn String v8wrapper/run) script))
