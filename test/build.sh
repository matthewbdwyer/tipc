#!/bin/sh
../build/tipc $1
clang -w -static $1.bc ../rtlib/tip_rtlib.bc -o `basename $1 .tip`
