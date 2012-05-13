(use 'net.n01se.clojure-jna)
(def jrun (jna-fn String v8wrapper/run))


(println (jrun "3/0"))

