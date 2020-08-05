#!/bin/bash

TIPC=../../build/src/tipc
RTLIB=../../rtlib

numtests=0
numfailures=0

curdir="$(basename `pwd`)"
if [ "${curdir}" != "system" ]; then
  echo "Test runner must be executed in .../tipc/test/system"
  exit 1
fi

# Self contained test cases
for i in selftests/*.tip
do
  base="$(basename $i .tip)"
  ((numtests++))

  ${TIPC} $i
  ${TIPCLANG} -w $i.bc ${RTLIB}/tip_rtlib.bc -o $base

  ./${base} >/dev/null 2>/dev/null
  exit_code=${?}
  if [ ${exit_code} -ne 0 ]; then
    echo -n "Test failure for : " 
    echo $i
    ./${base}
    ((numfailures++))
  else 
    rm ${base}
  fi 
  rm $i.bc
done

for i in selftests/*.tip
do
  base="$(basename $i .tip)"
  ((numtests++))

  ${TIPC} -do $i
  ${TIPCLANG} -w -static $i.bc ${RTLIB}/tip_rtlib.bc -o $base

  ./${base} >/dev/null 2>/dev/null
  exit_code=${?}
  if [ ${exit_code} -ne 0 ]; then
    echo -n "Test failure for unoptimized : "
    echo $i
    ./${base}
    ((numfailures++))
  else
    rm ${base}
  fi
  rm $i.bc
done

# IO related test cases
for i in iotests/*.expected
do
  expected="$(basename $i .tip)"
  executable="$(echo $expected | cut -f1 -d-)"
  input="$(echo $expected | cut -f2 -d- | cut -f1 -d.)"
  ((numtests++))

  ${TIPC} iotests/$executable.tip
  ${TIPCLANG} -w -static iotests/$executable.tip.bc ${RTLIB}/tip_rtlib.bc -o $executable

  ./${executable} $input >iotests/$executable.output

  diff iotests/$executable.output $i >/tmp/$executable.diff

  if [[ -s /tmp/$expected.diff ]]
  then
    echo -n "Test differences for : " 
    echo $i
    cat /tmp/$executable.diff
    ((numfailures++))
  fi 

  rm iotests/$executable.tip.bc
  rm iotests/$executable.output
  rm /tmp/$executable.diff
  rm $executable
done

# Tests to cover driver logic for error and argument handling
for i in iotests/*error.tip
do
  ((numtests++))

  ${TIPC} $i >/dev/null 2>/dev/null
  exit_code=${?}
  if [ ${exit_code} -eq 0 ]; then
    echo -n "Test failure for : " 
    echo -n $i
    echo " expected error"
    ((numfailures++))
    rm iotests/*error.tip.bc
  fi 
done

# Tests to cover argument handling
((numtests++))
${TIPC} -pp -ps iotests/fib.tip >/tmp/fib.ppps
diff iotests/fib.ppps /tmp/fib.ppps >/tmp/fib.diff
if [[ -s /tmp/fib.diff ]]
then
  echo -n "Test differences for : " 
  echo $i
  cat fib.diff
  ((numfailures++))
fi 

if [ ${numfailures} -eq "0" ]; then
  echo -n "all " 
  echo -n ${numtests}
  echo " tests passed"
else
  echo -n ${numfailures}/${numtests}
  echo " tests failed"
fi

