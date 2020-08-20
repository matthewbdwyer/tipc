#!/bin/sh
if [ -z "${TIPCLANG}" ]; then
  echo error: TIPCLANG env var must be set
  exit 1
fi

${TIPCLANG} -c -emit-llvm tip_rtlib.c
