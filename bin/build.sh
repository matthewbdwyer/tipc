#!/usr/bin/env bash
declare -r ROOT_DIR=${TIPDIR:-$(git rev-parse --show-toplevel)}
declare -r TIPC=${ROOT_DIR}/build/src/tipc
declare -r RTLIB=${ROOT_DIR}/rtlib

${TIPC} $@
${TIPCLANG} -w ${@: -1}.bc ${RTLIB}/tip_rtlib.bc -o `basename ${@: -1} .tip`
