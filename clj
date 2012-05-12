#!/bin/sh
# Clojure wrapper script.

CLOJURE=$CLASSPATH:/usr/local/Cellar/clojure/1.4.0/clojure-1.4.0.jar:${PWD}:lib/jna.jar:lib/platform.jar:clojure-jna/src

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./build/native/macosx/x86_64:./build/native/linux/x86:./build/native/linux/x86_64
export DYLD_LIBRARY_PATH=./build/native/macosx/x86_64:./build/native/linux/x86:./build/native/linux/x86_64

if [ "$#" -eq 0 ]; then
    java -cp "$CLOJURE" clojure.main --repl
else
    java -cp "$CLOJURE" clojure.main "$@"
fi
