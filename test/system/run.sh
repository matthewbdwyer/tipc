#!/bin/bash
declare -r ROOT_DIR=${TRAVIS_BUILD_DIR:-$(git rev-parse --show-toplevel)}
declare -r TIPC=${ROOT_DIR}/build/src/tipc
declare -r RTLIB=${ROOT_DIR}/rtlib
declare -r SCRATCH_DIR=$(mktemp -d)

if [ -z "${TIPCLANG}" ]; then
  echo error: TIPCLANG env var must be set
  exit 1
fi

curdir="$(basename `pwd`)"
if [ "${curdir}" != "system" ]; then
  echo "Test runner must be executed in .../tipc/test/system"
  exit 1
fi

numtests=0
numfailures=0

initialize_test() {
  rm -f ${SCRATCH_DIR}/*
  ((numtests++))
}

# Self contained test cases
for i in selftests/*.tip
do
  base="$(basename $i .tip)"

  # test optimized program
  initialize_test
  ${TIPC} $i
  ${TIPCLANG} $i.bc ${RTLIB}/tip_rtlib.bc -o $base

  ./${base} &>/dev/null
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

  # test unoptimized program
  initialize_test
  ${TIPC} -do $i
  ${TIPCLANG} $i.bc ${RTLIB}/tip_rtlib.bc -o $base

  ./${base} &>/dev/null
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

# IO related test cases
for i in iotests/*.expected
do
  initialize_test
  expected="$(basename $i .tip)"
  executable="$(echo $expected | cut -f1 -d-)"
  input="$(echo $expected | cut -f2 -d- | cut -f1 -d.)"

  ${TIPC} iotests/$executable.tip
  ${TIPCLANG} iotests/$executable.tip.bc ${RTLIB}/tip_rtlib.bc -o $executable

  ./${executable} $input >iotests/$executable.output 2>iotests/$executable.output

  diff iotests/$executable.output $i > ${SCRATCH_DIR}/$executable.diff

  if [[ -s ${SCRATCH_DIR}/$executable.diff ]]
  then
    echo -n "Test differences for : " 
    echo $i
    cat ${SCRATCH_DIR}/$executable.diff
    ((numfailures++))
  fi 

  rm iotests/$executable.tip.bc
  rm iotests/$executable.output
  rm $executable
done

# Tests to cover driver logic for error and argument handling
for i in iotests/*error.tip
do
  initialize_test

  ${TIPC} $i &>/dev/null
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
# Test pretty printing and symbol printing.
initialize_test
${TIPC} -pp -ps iotests/fib.tip >${SCRATCH_DIR}/fib.ppps
diff iotests/fib.ppps ${SCRATCH_DIR}/fib.ppps >${SCRATCH_DIR}/fib.diff
if [[ -s ${SCRATCH_DIR}/fib.diff ]]
then
  echo -n "Test differences for : " 
  echo $i
  cat fib.diff
  ((numfailures++))
fi 

# Test default output file.
initialize_test
input=iotests/main.tip
expected=iotests/main.tip.ll
${TIPC} --asm $input
if [ ! -f $expected ]; then
  echo -n "Did not find exepected output, $expected, for input $input" 
  ((numfailures++))
fi 
rm $expected

# Test human-readable assembly.
initialize_test
input=iotests/fib.tip
output=${SCRATCH_DIR}/fib.tip.ll
expected=iotests/fib.tip.ll
diffed=${SCRATCH_DIR}/fib.diff
${TIPC} --asm $input -o $output
diff <(sed -n '4,$p' $output) <(sed -n '4,$p' $expected) > $diffed
if [ -s $diffed ]; then
  echo -n "Test differences for: $input" 
  cat $diffed
  ((numfailures++))
fi 

# Test call graph.
initialize_test
input=iotests/fib.tip
output=${SCRATCH_DIR}/fib.tip.bc
output_graph=${SCRATCH_DIR}/fib.tip.gv
expected_graph=iotests/fib.tip.gv
diffed_graph=${SCRATCH_DIR}/fib.tip.diff
${TIPC} --pcg $input -o $output > $output_graph
diff $output_graph $expected_graph > $diffed_graph
if [ -s $diffed_graph ]; then
  echo "Test differences for: $input" 
  cat $diffed_graph
  ((numfailures++))
fi 

# Test bad input.
initialize_test
nonexistent=$(uuidgen).tip
while [ -e $nonexistent ]; do
  nonexistent=$(uuidgen).tip
done

${TIPC} $nonexistent &>/dev/null
exit_code=${?}
if [ ${exit_code} -eq 0 ]; then
  echo -n "Test failure for non-exisitent input" 
  ((numfailures++))
fi

# Type checking at the system level
for i in selftests/*.tip
do
  initialize_test
  base="$(basename $i .tip)"

  ${TIPC} -pp -pt $i >${SCRATCH_DIR}/$base.pppt
  diff $i.pppt ${SCRATCH_DIR}/$base.pppt >${SCRATCH_DIR}/$base.diff
  if [[ -s ${SCRATCH_DIR}/$base.diff ]]
  then
    echo -n "Test differences for : " 
    echo $i
    cat ${SCRATCH_DIR}/$base.diff
    ((numfailures++))
  fi 
done

# Logging test 
#   kick the tires on logging to make sure there are null pointer derefs
initialize_test
${TIPC} -pt -log=/dev/null selftests/polyfactorial.tip &>/dev/null 

if [ ${numfailures} -eq "0" ]; then
  echo -n "all " 
  echo -n ${numtests}
  echo " tests passed"
else
  echo -n ${numfailures}/${numtests}
  echo " tests failed"
fi

rm -r ${SCRATCH_DIR}
