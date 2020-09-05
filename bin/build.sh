#!/usr/bin/env bash
set -e
declare -r ROOT_DIR=${TIPDIR:-$(git rev-parse --show-toplevel)}
declare -r TIPC=${ROOT_DIR}/build/src/tipc
declare -r RTLIB=${ROOT_DIR}/rtlib

if [ -z "${TIPCLANG}" ]; then
  echo error: TIPCLANG env var must be set
  exit 1
fi

if [ ! -f "${TIPC}" ]; then
  echo error: tipc was not found
  exit 1
fi

if [ ! -f "${RTLIB}/tip_rtlib.bc" ]; then
  echo error: tip_rtlib.bc was not found
  exit 1
fi

${TIPC} $@
${TIPCLANG} -w ${@: -1}.bc ${RTLIB}/tip_rtlib.bc -o `basename ${@: -1} .tip`
