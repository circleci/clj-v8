(ns v8.test-core
  (:require [v8.core :as v8])
  (:use [midje.sweet]))

(fact "run-script works"
  (v8/run-script "3/0") => "Infinity")

(fact "complex code works"
  (v8/run-script "(function(){ return 5; })();") => "5")

(fact "syntax error doesnt die"
  (v8/run-script "sjd2-23=dfv;2-") => "")