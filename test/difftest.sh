#!/bin/sh
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

# We are only interested in differences related to computed outputs.
# If the diff contains none, then the test passe
if diff /tmp/$bname.tipc-out /tmp/$bname.tipscala-out | grep -q "output"; then
  echo "$bname failed"
else
  echo "$bname passed"
fi
