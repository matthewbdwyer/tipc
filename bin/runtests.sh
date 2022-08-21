#!/usr/bin/env bash
set -e
declare -r ROOT_DIR=${GITHUB_WORKSPACE:-$(git rev-parse --show-toplevel)}
declare -r RTLIB_DIR=${ROOT_DIR}/rtlib
declare -r UNIT_TEST_DIR=${ROOT_DIR}/build/test/unit
declare -r SYSTEM_TEST_DIR=${ROOT_DIR}/test/system

usage() { 
  echo "usage: $0 [-h] [-s] [-u]" 1>&2;
  echo "run the complete unit and system test suite"
  echo
  echo "-h  display help"
  echo "-s  runs system tests only"
  echo "-u  runs unit tests only"
}

run_unit_tests() {
  find ${ROOT_DIR} -name '*gcda' -delete
  echo running the unit test suite
  find ${UNIT_TEST_DIR} -name '*_unit_tests' | xargs -n1 sh -c
  echo unit test run complete
}

assert_unit_test_dir() {
  if [ ! -d ${UNIT_TEST_DIR} ]; then
    echo ${UNIT_TEST_DIR} was not found. Please make sure you build the project before running tests.
    exit 1
  fi
}

run_system_tests() {
  pushd ${RTLIB_DIR} &> /dev/null
  ./build.sh
  if [ $? -ne 0 ]; then
    echo error: could not build the runtime library
    exit 1
  fi
  popd &> /dev/null

  echo running the system test suite
  pushd ${SYSTEM_TEST_DIR} &> /dev/null
  ./run.sh
  if [ $? -ne 0 ]; then
    echo error while running system tests
    exit 1
  fi
  popd &> /dev/null
  echo system test suite complete
}

run_system_tests="true"
run_unit_tests="true"
while getopts ":hsu" opt; do
  case "${opt}" in
    h)
      usage
      exit 0
      ;;
    s)
      echo Preparing to run only the system tests suite
      run_unit_tests=""
      ;;
    u)
      echo Preparing to run only the unit tests suite
      run_system_tests=""
      ;;
    *)
      echo $0 illegal option
      usage
      exit 1
      ;;
  esac
done
shift $((OPTIND-1))

if [ -n "${run_unit_tests}" ]; then
  assert_unit_test_dir
  run_unit_tests
fi

if [ -n "${run_system_tests}" ]; then
  run_system_tests
fi
