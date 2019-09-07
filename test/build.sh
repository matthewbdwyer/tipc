#/bin/sh
../build/tipc $1
clang-7 -static $1.bc ../intrinsics/tip_intrinsics.bc -o `basename $1 .tip`
