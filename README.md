clj-v8
======

Clojure wrapper for the v8 JS engine


Organization
+++++++++++++++

clj-v8 contains the clj-v8 jar. clj-v8-native contains the native deps only



Rebuilding from scratch
++++++++++++++++++++++++++

TL;DR
-----
The short version:

0. Make sure you have
   - svn
   - git
   - a C++ toolchain (for Linux, you need a 64 bit machine with
     a multilib C++ compiler)

1. Unpack the tarball

2. Get into the directory which you unpacked and just do a ./build.sh
   It'll put all the binary artefacts into ./build
   You'll need to do it once on OSX and once on a 64 bit Linux box

3. Get the build dirs from the two build machines into one place
   then call ./pack.sh

What follows below is an indepth explanation of how the build process
actually works and what it does.


DOWNLOADING AND DEPS
--------------------

First off, check out the latest trunk version of the V8 engine, and
get into that shit

$ git clone git://github.com/v8/v8.git v8
$ cd v8

Since some time ago V8 is using GYP instead of scons as its build
platform... but that pulls in a shitload of deps. Fortunately, we can
do:

$ make dependencies

God bless them, it downloads all of the fluff. As if we needed another
build platform, and one written in Python at that.

This is basically where you have to diverge a little bit for OSX


OS X 10.7
---------

A problem that seems to be occuring is that no matter which
architecture is chosen through make, the gyp build rules fall back to
"i386" as the architecture sent to the compiler. This tries to compile
a 32 bit version.

I had to patch that part out because later on, the x64 target tests
for the presence of 64 bit registers and as i386 was forced, the build
fails pretty horribly and with non-descriptive errors.

I'm not sure if this has any long-term implications, but the resulting
changes build a proper x86_64 dylib on 10.7. This will probably
produce mangled builds or fail if you try building on non 64bit OS X
computers, but this seems to be inconsequential or of historical
interest only.

After checking out the v8 code from git, patch the sources using the
osx-arch.diff file.


Back to all platforms: building V8
----------------------------------

After that dirty patching business, we're back to normal and should
have everything working the same on all platforms.

For 64 bit builds on Linux and OSX use:

$ make library=shared x64.release

While the 32 bit Linux build will require a somewhat different, but
not unexpected:

$ make library=shared ia32.release

... lots of time passes during the compilation ...

Now you are ready to build the accessories and pack everything up...


Building the pure-C V8 wrapper
------------------------------

Handling V8's C++ API is a pain from JNA, but you can easily wrap that
C++ code in some pure C. That's what I did, and that part is contained
in the v8wrapper directory of the installation package.

You can compile it by calling make and -f-ing it with the proper
Makefile for your system (Makefile.macosx for OSX and Makefile.linux,
Makefile.linux.32 for Linux).


Putting it all together
-----------------------

Once you have all the binaries for all the achitectures in your build
dir on one machine, just call pack.sh from the root of the build tree:

$ ./pack.sh

This results in a jar which contains both some simple wrapper code and
the native deps that we previously built.

Combine that jar file with the pom.xml located in the resources
directory and you will be able to upload this to Clojars or any other
similar service.


Running an example
------------------
