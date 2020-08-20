#!/bin/sh
declare -r TIPC=${ROOT_DIR}/build/src/tipc
declare -r RTLIB=${TRAVIS_BUILD_DIR}/rtlib

${TIPC} $1
${TIPCLANG} -w $1.bc ${RTLIB}/tip_rtlib.bc -o `basename $1 .tip`
