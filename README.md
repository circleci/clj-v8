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

Copyright (c) the clj-v8 project authors. All rights reserved.

The use and distribution terms for this software are covered by the
Eclipse Public License 1.0 (http://opensource.org/licenses/eclipse-1.0.php),
the same as Clojure.

By using this software in any fashion, you are agreeing to be bound by the
terms of this license. You must not remove this notice, or any other, from this
software.

This license applies to this distribution and all prior distributions of clj-v8.

This license applies to all parts of clj-v8 that are not externally maintained
libraries.

Distributions of clj-v8 may include external software from the V8 project, which
has this notice and terms:

> Copyright 2014, the V8 project authors. All rights reserved.
> Redistribution and use in source and binary forms, with or without
> modification, are permitted provided that the following conditions are
> met:
> 
> * Redistributions of source code must retain the above copyright
> notice, this list of conditions and the following disclaimer.
> * Redistributions in binary form must reproduce the above
> copyright notice, this list of conditions and the following
> disclaimer in the documentation and/or other materials provided
> with the distribution.
> * Neither the name of Google Inc. nor the names of its
> contributors may be used to endorse or promote products derived
> from this software without specific prior written permission.
> 
> THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
> "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
> LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
> A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
> OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
> SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
> LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
> DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
> THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
> (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
> OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
