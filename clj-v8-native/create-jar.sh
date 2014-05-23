#!/bin/bash

VERSION=`grep '<version>' pom.xml | sed -Ee 's#.*<version>(.*)</version>.*#\1#'`

rm -f clj-v8-native-$VERSION.jar
jar -cMf clj-v8-native-$VERSION.jar -C build native pom.xml
