#!/bin/bash

function create_output_dirs {
    mkdir -p build/native/macosx-x86-64
    mkdir -p build/native/linux-{x86,x86-64}
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
        make debuggersupport=off library=shared x64.release
    else
        echo Building Linux 32 bit
        make debuggersupport=off library=shared ia32.release

        echo Building Linux 64 bit
        make debuggersupport=off library=shared x64.release
    fi
}

function build_and_copy_v8w {
    cd src/v8wrapper

    if [ "$PLATFORM" = "macosx" ]; then
        make -f Makefile.$PLATFORM clean all

        echo "=== Copying v8wrapper, v8, and icu ==="
	cp libicui18n.dylib    ../../build/native/macosx-x86-64/
	cp libicuuc.dylib      ../../build/native/macosx-x86-64/
        cp libv8.dylib         ../../build/native/macosx-x86-64/
        cp libv8wrapper.dylib  ../../build/native/macosx-x86-64/	
    else
        make -f Makefile.$PLATFORM clean all
	
        echo "=== Copying v8wrapper, v8, and icu (64 bit) ==="
	cp libicui18n.so    ../../build/native/linux-x86-64/
	cp libicuuc.so      ../../build/native/linux-x86-64/
        cp libv8.so         ../../build/native/linux-x86-64/
        cp libv8wrapper.so  ../../build/native/linux-x86-64/
	
        make -f Makefile.$PLATFORM.32 clean all
	
        echo "=== Copying v8wrapper, v8, and icu (32 bit) ==="
	cp libicui18n.so    ../../build/native/linux-x86/
	cp libicuuc.so      ../../build/native/linux-x86/
	cp libv8.so         ../../build/native/linux-x86/
        cp libv8wrapper.so  ../../build/native/linux-x86/	
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
