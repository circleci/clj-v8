(ns v8.core-test
  (:require [clojure.test :refer :all]
            [v8.core :as v8]))

(v8/set-flags "--harmony")

(deftest set-flags-test
  (testing "it works"
    (is (= (v8/run-script "const x = 45; x;") "45"))))

(deftest run-script-test
  (testing "it works"
   (is (= (v8/run-script "123") "123"))
   (is (= (v8/run-script "readFile('test/v8/test.txt')") "123\n")))
  
  (testing "simple expressions"
    (is (= (v8/run-script "3/0") "Infinity")))
  
  (testing "complex code works"
    (is (= (v8/run-script "(function(){ return 5; })();")) "5"))
  
  (testing "Unicode won't ☔ on my parade"
    (is (= (v8/run-script "\"šećiđon☔\"") "šećiđon☔")))

  (testing "syntax error doesnt die"
    (is (thrown? Exception (v8/run-script "sjd2-23=dfv;2-"))))

  (testing "multuple scripts in parallel work"
    (is (= (pmap v8/run-script (repeat 20 "(function(){ return 5; })();"))
           (repeat 20 "5"))))
  
  (testing "running two scripts on same context"
    (let [cx (v8/create-context)]
      (v8/run-script-in-context cx "x = 17; y = {a: 6};")
      (is (= (v8/run-script-in-context cx "x;") "17"))
      (is (= (v8/run-script-in-context cx "y.a;") "6")))))

(deftest initialize-icu-test
  (testing "it works"
    (v8/initialize-icu)))


