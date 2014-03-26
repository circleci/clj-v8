clj-v8
======

Clojure wrapper for the v8 JS engine


Organization
-----------------

clj-v8 contains the clj-v8 jar. clj-v8-native contains the native deps only.


Rebuilding from scratch
---------------------------

The short version:

0. Make sure you have
   - svn
   - git
   - a C++ toolchain (for Linux, you need a 64 bit machine with
     a multilib C++ compiler)

1. Unpack the tarball

2. Get into the directory which you unpacked and just do a ./build.sh
   It'll put all the binary artefacts into ./build
   You'll need to do it once on OSX and once on a 64 bit Linux box, but we keep
   the built files in the repository for ease of cross-platform development.

3. Get the build dirs from the two build machines into one place
   then call ./create-jar.sh


Read build.sh to see how it all fits together.


Changelog
---------

#### From 0.1.4 to 0.1.5

- clj-v8 can now be bundled in uberjars. Binaries are copied out of
  the JAR to a temporary directory to be available for JNI. (Magnar Sveen)


Authors
--------

clj-v8 and clj-v8-wrapper was built by:

- Paul Biggar <paul@circleci.com>, Ivan Stojic <ivans@ordecon.com> from CircleCI (https://circleci.com)
- Magnar Sveen (https://github.com/magnars)
- Abhishek Reddy (https://github.com/arbscht)

License
--------

Distributed under the Eclipse Public License, the same as Clojure uses.

This license applies to this distribution and all prior distributions of clj-v8.
