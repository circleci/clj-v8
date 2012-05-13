#!/bin/bash

function create_output_dirs {
    rm -rf build

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
            exit
        fi

    else
        echo "I don't know how to build/package everything on `uname` platform. Sorry"
        exit
    fi
}

function build_v8 {
    if [[ -e "v8" ]]; then
        cd v8
        git pull
    else
        git clone git://github.com/v8/v8.git v8
        cd v8
    fi

    make dependencies

    if [ "$PLATFORM" = "macosx" ]; then
        echo Building Mac OS X 64 bit
        patch -p1 < ../osx-arch.diff
        make library=shared x64.release

    else
        echo Building Linux 32 bit
        make library=shared ia32.release

        echo Building Linux 64 bit
        make library=shared x64.release
    fi
}

function build_and_copy_v8w {
    cd src/v8wrapper

    if [ "$PLATFORM" = "macosx" ]; then
        make -f Makefile.$PLATFORM clean all

        cp libv8wrapper.dylib ../../build/native/macosx/x86_64
        install_name_tool -id libv8wrapper.dylib ../../build/native/macosx/x86_64/libv8wrapper.dylib

    else
        make -f Makefile.$PLATFORM clean all
        cp libv8wrapper.so ../../build/native/linux/x86_64

        make -f Makefile.$PLATFORM.32 clean all
        cp libv8wrapper.so ../../build/native/linux/x86
    fi
}

function copy_v8 {
    if [ "$PLATFORM" = "macosx" ]; then
        cp v8/out/x64.release/libv8.dylib ./build/native/macosx/x86_64/
        install_name_tool -id libv8.dylib ./build/native/macosx/x86_64/libv8.dylib

    else
        cp v8/out/x64.release/lib.target/libv8.so ./build/native/linux/x86_64/
        cp v8/out/ia32.release/lib.target/libv8.so ./build/native/linux/x86/
    fi
}

echo "==== Detecting environment ===="
detect

echo "==== Cleaning up ===="
create_output_dirs

# echo "==== Building V8 ===="
(build_v8)
(copy_v8)

echo "==== Building V8W ===="
(build_and_copy_v8w)
