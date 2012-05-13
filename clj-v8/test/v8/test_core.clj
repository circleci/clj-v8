(ns v8.test-core
  (:require [v8.core :as v8])
  (:use [midje.sweet]))

(fact "run-script works"
  (with-out-str (v8/run-script "3/0")) => "Invalid")