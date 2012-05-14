(defproject clj-v8 "0.1.0"
  :description "A Clojure wrapper for the v8 Javascript library"
  :dependencies [[org.clojars.elmom/clojure-jna "0.9"]]
  :native-dependencies [[clj-v8-native "0.1.0"]]
  :dev-dependencies [[midje "1.3.1"]
                     [redd/native-deps "1.0.7"]]
  :jvm-opts ["-Djna.library.path=native/macosx/x86_64"])