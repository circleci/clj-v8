#!/bin/bash

set -e

VERSION=`grep '<version>' pom.xml | sed -e 's#.*<version>##' -e 's#</version>.*##'`

rm -f clj-v8-native-$VERSION.jar
jar -cMf clj-v8-native-$VERSION.jar -C build/ lib -C build native pom.xml
