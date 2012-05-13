#!/bin/bash

jar -cMf clj-v8-native-0.1.1.jar -C build/ lib -C build native pom.xml
