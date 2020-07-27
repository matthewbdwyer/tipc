#!/bin/sh
../../build/src/tipc $1
${TIPCLANG} -w $1.bc ../../rtlib/tip_rtlib.bc -o `basename $1 .tip`
