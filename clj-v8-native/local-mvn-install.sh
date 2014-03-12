#!/bin/bash

VERSION=`grep '<version>' pom.xml | sed -e 's#.*<version>##' -e 's#</version>.*##'`
MVNPATH=~/.m2/repository/clj-v8-native/clj-v8-native/$VERSION

mkdir -p $MVNPATH
rm -f $MVNPATH/*

cp clj-v8-native-$VERSION.jar $MVNPATH
cp pom.xml $MVNPATH/clj-v8-native-$VERSION.pom

