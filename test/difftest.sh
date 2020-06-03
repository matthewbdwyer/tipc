#!/bin/sh
if [ $# -lt 1 ]; then
  echo "$0: you must provide a .tip file"
  exit 0;
fi

bname=`basename $1 .tip`

# create a per user tmp directory for storing diff files
if [ ! -d /tmp/$USER ]; then
  mkdir /tmp/$USER
fi

# compile and run the test through tipc 
#    we suppress warnings while linking because of a target triple mismatch
../build/tipc $1
clang -w -static $1.bc ../rtlib/tip_rtlib.bc -o $bname
./$bname >/tmp/$USER/$bname.tipc-out

# create a tipc directory for storing source to run TIP Scala
if [ ! -d ~/TIP/tipc ]; then
  mkdir ~/TIP/tipc
fi

# run the test through TIP Scala 
#   must execute in its build directory
#   reenable stty echo when finished
cp $1 ~/TIP/tipc/
cd ~/TIP
./tip -run tipc/$1 >/tmp/$USER/$bname.tipscala-out
stty echo
cd - >/dev/null

# We are only interested in differences related to computed outputs
# and errors. If the diff contains none, then the test passes
# We have to transform the scala output to remove specific unprintable
# chars prior to diffing.
sed 's/\[\(0\|1\|31\)m//g' /tmp/$USER/$bname.tipscala-out >/tmp/$USER/t
cp /tmp/$USER/t /tmp/$USER/$bname.tipscala-out
if diff /tmp/$USER/$bname.tipc-out /tmp/$USER/$bname.tipscala-out | grep -q -e "Program output" -e " Error: Execution error"; then
  echo "$bname failed"
else
  echo "$bname passed"
fi
