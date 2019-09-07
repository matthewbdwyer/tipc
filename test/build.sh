#/bin/sh
../build/tipc $1
clang -static $1.bc ../intrinsics/tip_intrinsics.bc -o `basename $1 .tip`
