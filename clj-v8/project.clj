(defproject clj-v8 "0.1.5"
  :description "A Clojure wrapper for the v8 Javascript library"
  :license {:name "Eclipse Public License 1.0"
            :url "http://opensource.org/licenses/eclipse-1.0.php"
  :dependencies [[org.clojure/clojure "1.3.0"]
                 [net.java.dev.jna/jna "3.5.2"]
                 [clj-v8-native/clj-v8-native "0.1.4"]]
  :dev-dependencies [[midje "1.3.1"]]
  :profiles {:dev {:dependencies [[midje "1.3.1" :exclusions [org.clojure/clojure]]]}}
  :jvm-opts ["-Djava.library.path=target/native/macosx/x86_64:target/native/linux-rhel5/x86_64:target/native/linux/x86_64:target/native/linux/x86:native/macosx/x86_64:native/linux-rhel5/x86_64:native/linux/x86_64:native/linux/x86:"])
