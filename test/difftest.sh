#!/bin/sh
if [ $# -lt 1 ]; then
  echo "$0: you must provide a .tip file"
  exit 0;
fi

bname=`basename $1 .tip`

# compile and run the test through tipc 
#    we suppress warnings while linking because of a target triple mismatch
../build/tipc $1
clang-7 -w -static $1.bc ../intrinsics/tip_intrinsics.bc -o $bname
./$bname >/tmp/$bname.tipc-out

# run the test through TIP Scala 
#   must execute in its build directory
#   reenable stty echo when finished
cp $1 ~/TIP/tipc
cd ~/TIP
./tip -run tipc/$1 >/tmp/$bname.tipscala-out
stty echo
cd - >/dev/null

# We are only interested in differences related to computed outputs
# and errors. If the diff contains none, then the test passes
# We have to transform the scala output to remove specific unprintable
# chars prior to diffing.
sed 's/\[\(0\|1\|31\)m//g' /tmp/$bname.tipscala-out >/tmp/t
cp /tmp/t /tmp/$bname.tipscala-out
if diff /tmp/$bname.tipc-out /tmp/$bname.tipscala-out | grep -q -e "Program output" -e " Error: Execution error"; then
  echo "$bname failed"
else
  echo "$bname passed"
fi
