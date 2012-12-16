(defproject clj-v8 "0.1.3"
  :description "A Clojure wrapper for the v8 Javascript library"
  :dependencies [[org.clojure/clojure "1.3.0"]
                 [org.clojars.elmom/clojure-jna "0.9"]
                 [clj-v8-native/clj-v8-native "0.1.3"]]
  :dev-dependencies [[midje "1.3.1"]]
  :profiles {:dev {:dependencies [[midje "1.3.1" :exclusions [org.clojure/clojure]]]}}
  :jvm-opts ["-Djna.library.path=target/native/macosx/x86_64:target/native/linux/x86_64:target/native/linux/x86:native/macosx/x86_64:native/linux/x86_64:native/linux/x86:"])
