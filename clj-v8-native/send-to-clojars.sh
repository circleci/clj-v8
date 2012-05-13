#!/bin/bash

set -x

scp -i ~/.leiningen/id_rsa pom.xml *.jar clojars@clojars.org:
