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
../build/tipc $1
cd ~/tipc/build/
./tipc -t ../testtype/$1 >/tmp/$USER/$bname.tipc-type-out

# create a tipc directory for storing source to run TIP Scala
if [ ! -d ~/TIP/tipc ]; then
  mkdir ~/TIP/tipc
fi

# run the test through TIP Scala 
#   must execute in its build directory
#   reenable stty echo when finished
cp ~/tipc/testtype/$1 ~/TIP/tipc/
cd ~/TIP
./tip -types tipc/$1
cat ./out/$1__types.ttip >/tmp/$USER/$bname.tipscala-type-out
stty echo
cd - >/dev/null

# We are only interested in differences related to computed outputs
# and errors. If the diff contains none, then the test passes
# We have to transform the scala output to remove specific unprintable
# chars prior to diffing.
sed 's/\[\(0\|1\|31\)m//g' /tmp/$USER/$bname.tipscala-type-out >/tmp/$USER/t
cp /tmp/$USER/t /tmp/$USER/$bname.tipscala-type-out
diff -w -B /tmp/$USER/$bname.tipc-type-out /tmp/$USER/$bname.tipscala-type-out
ret=$?

if [ $ret -eq 0 ]; then
  echo "$bname passed"
else
  echo "$bname failed"
fi