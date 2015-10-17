#!/bin/bash

set -euo pipefail

function create_output_dirs {
    mkdir -p build/lib
    mkdir -p build/native/{macosx,linux}
    mkdir -p build/native/macosx/x86_64
    mkdir -p build/native/linux/{x86,x86_64}
}

function detect {
    if [ "`uname`" = "Darwin" ]; then
        PLATFORM=macosx

    elif [ "`uname`" = "Linux" ]; then
        PLATFORM=linux

        if [ "`uname -m`" != "x86_64" ]; then
            echo "This is a 32-bit only Linux. I'd suggest you find a 64-bit build box. Sorry."
            exit 1
        fi

    else
        echo "I don't know how to build/package everything on `uname` platform. Sorry"
        exit 1
    fi
}

function build_v8 {
    cd src/v8
    make dependencies

    if [ "$PLATFORM" = "macosx" ]; then
        echo Building Mac OS X 64 bit
        make debuggersupport=off library=shared soname_version=clj-v8 x64.release
    else
        echo Building Linux 32 bit
        make debuggersupport=off library=shared soname_version=clj-v8 ia32.release

        echo Building Linux 64 bit
        make debuggersupport=off library=shared soname_version=clj-v8 x64.release
    fi
}

function build_and_copy_v8w {
    cd src/v8wrapper

    if [ "$PLATFORM" = "macosx" ]; then
        make -f Makefile.$PLATFORM clean all

        echo "=== Copying v8wrapper and v8 === "
        cp libv8wrapper.dylib ../../build/native/macosx/x86_64
        cp libv8.dylib.clj-v8 ../../build/native/macosx/x86_64/
    else
        make -f Makefile.$PLATFORM clean all
        echo "=== Copying v8wrapper and v8 (64bit) === "
        cp libv8wrapper.so ../../build/native/linux/x86_64
        cp libv8.so.clj-v8 ../../build/native/linux/x86_64/

        make -f Makefile.$PLATFORM.32 clean all
        echo "=== Copying v8wrapper and v8 (32bit) === "
        cp libv8wrapper.so ../../build/native/linux/x86
        cp libv8.so.clj-v8 ../../build/native/linux/x86/
    fi
}

echo "==== Detecting environment ===="
detect

echo "==== Preparing ===="
create_output_dirs

echo "==== Building v8 ===="
(build_v8)

echo "==== Building v8wrapper ===="
(build_and_copy_v8w)
