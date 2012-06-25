(defproject clj-v8 "0.1.1"
  :description "A Clojure wrapper for the v8 Javascript library"
  :dependencies [[org.clojure/clojure "1.3.0"]
                 [org.clojars.elmom/clojure-jna "0.9"]
                 [clj-v8-native/clj-v8-native "0.1.1"]]
  :dev-dependencies [[midje "1.3.1"]]
  :jvm-opts ["-Djna.library.path=native/macosx/x86_64:native/linux/x86_64:native/linux/x86:"])

