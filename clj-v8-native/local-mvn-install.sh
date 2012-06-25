#!/bin/bash

VERSION=`grep '<version>' pom.xml | sed -Ee 's#.*<version>(.*)</version>.*#\1#'`
MVNPATH=~/.m2/repository/clj-v8-native/clj-v8-native/$VERSION

mkdir -p $MVNPATH
rm -f $MVNPATH/*

cp clj-v8-native-$VERSION.jar $MVNPATH
cp pom.xml $MVNPATH/clj-v8-native-$VERSION.pom

