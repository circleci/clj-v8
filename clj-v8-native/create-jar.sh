#!/bin/bash

rm -f clj-v8-native-0.1.2.jar
jar -cMf clj-v8-native-0.1.2.jar -C build/ lib -C build native pom.xml
