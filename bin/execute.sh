#!/bin/bash

export TIPCLANG=$(which clang-10)

./build.sh $1
./`basename ${@:-1} .tip`