#!/bin/bash

TIPC=../build/tipc
RTLIB=../rtlib

numtests=0
numfailures=0

for i in selftests/*.tip
do
  base="$(basename $i .tip)"
  ((numtests++))

  ${TIPC} $i
  clang-10 -w -static $i.bc ${RTLIB}/tip_rtlib.bc -o $base

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

  ${TIPC} -d $i
  clang-10 -w -static $i.bc ${RTLIB}/tip_rtlib.bc -o $base

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


if [ ${numfailures} -eq "0" ]; then
  echo "pass"
else
  echo -n "fail : "
  echo -n ${numfailures}/${numtests}
  echo " tests failed"
fi

