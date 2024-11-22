#!/bin/bash

# Set up environment variables
declare -r ROOT_DIR=${TRAVIS_BUILD_DIR:-$(git rev-parse --show-toplevel)}
declare -r TIPC=${ROOT_DIR}/build/src/tipc
declare -r RTLIB=${ROOT_DIR}/rtlib

if [ -z "${TIPCLANG}" ]; then
  echo "error: TIPCLANG env var must be set"
  exit 1
fi

curdir="$(basename "$(pwd)")"
if [ "${curdir}" != "system" ]; then
  echo "Test runner must be executed in .../tipc/test/system"
  exit 1
fi

# Initialize counters
numtests=0
numfailures=0

# Export variables and functions for GNU parallel
export ROOT_DIR TIPC RTLIB TIPCLANG

# Function to run self-contained tests
run_selftest() {
  testfile="$1"
  base="$(basename "$testfile" .tip)"
  failures=0

  for optflag in '' '-do'; do
    # Create a temporary directory for this test
    SCRATCH_DIR=$(mktemp -d)

    # Compile the test file
    ${TIPC} $optflag "$testfile" &>/dev/null
    if [ $? -ne 0 ]; then
      echo "Compilation failed for: $testfile $optflag" >&2
      failures=$((failures + 1))
      rm -rf "$SCRATCH_DIR"
      continue
    fi

    # Link and build executable
    ${TIPCLANG} -w "$testfile.bc" "${RTLIB}/tip_rtlib.bc" -o "$SCRATCH_DIR/$base" &>/dev/null
    if [ $? -ne 0 ]; then
      echo "Linking failed for: $testfile $optflag" >&2
      failures=$((failures + 1))
      rm -f "$testfile.bc"
      rm -rf "$SCRATCH_DIR"
      continue
    fi

    # Run the executable
    "$SCRATCH_DIR/$base" &>/dev/null
    exit_code=$?
    if [ $exit_code -ne 0 ]; then
      echo "Test failure for: $testfile $optflag" >&2
      "$SCRATCH_DIR/$base" >&2
      failures=$((failures + 1))
    fi

    # Cleanup
    rm -f "$testfile.bc"
    rm -rf "$SCRATCH_DIR"
  done

  echo $failures
}

# Function to run SIP tests
run_siptest() {
  testfile="$1"
  base="$(basename "$testfile" .sip)"
  failures=0

  for optflag in '' '-do'; do
    SCRATCH_DIR=$(mktemp -d)

    ${TIPC} $optflag "$testfile" &>/dev/null
    if [ $? -ne 0 ]; then
      echo "Compilation failed for: $testfile $optflag" >&2
      failures=$((failures + 1))
      rm -rf "$SCRATCH_DIR"
      continue
    fi

    ${TIPCLANG} -w "$testfile.bc" "${RTLIB}/tip_rtlib.bc" -o "$SCRATCH_DIR/$base" &>/dev/null
    if [ $? -ne 0 ]; then
      echo "Linking failed for: $testfile $optflag" >&2
      failures=$((failures + 1))
      rm -f "$testfile.bc"
      rm -rf "$SCRATCH_DIR"
      continue
    fi

    "$SCRATCH_DIR/$base" &>/dev/null
    exit_code=$?
    if [ $exit_code -ne 0 ]; then
      echo "Test failure for: $testfile $optflag" >&2
      "$SCRATCH_DIR/$base" >&2
      failures=$((failures + 1))
    fi

    rm -f "$testfile.bc"
    rm -rf "$SCRATCH_DIR"
  done

  echo $failures
}

# Function to run IO tests
run_iotest() {
  expected_file="$1"
  failures=0

  expected="$(basename "$expected_file" .expected)"
  executable="$(echo "$expected" | cut -f1 -d-)"
  input="$(echo "$expected" | cut -f2 -d- | cut -f1 -d.)"

  SCRATCH_DIR=$(mktemp -d)
  ${TIPC} "iotests/$executable.tip" &>/dev/null
  if [ $? -ne 0 ]; then
    echo "Compilation failed for: $executable.tip" >&2
    failures=$((failures + 1))
    rm -rf "$SCRATCH_DIR"
    return
  fi

  ${TIPCLANG} -w "iotests/$executable.tip.bc" "${RTLIB}/tip_rtlib.bc" -o "$SCRATCH_DIR/$executable" &>/dev/null
  if [ $? -ne 0 ]; then
    echo "Linking failed for: $executable.tip.bc" >&2
    failures=$((failures + 1))
    rm -f "iotests/$executable.tip.bc"
    rm -rf "$SCRATCH_DIR"
    return
  fi

  "$SCRATCH_DIR/$executable" "$input" >"$SCRATCH_DIR/$executable.output" 2>&1

  diff "$SCRATCH_DIR/$executable.output" "$expected_file" >"$SCRATCH_DIR/$executable.diff"

  if [[ -s "$SCRATCH_DIR/$executable.diff" ]]; then
    echo "Test differences for: $expected_file" >&2
    cat "$SCRATCH_DIR/$executable.diff" >&2
    failures=$((failures + 1))
  fi

  rm -f "iotests/$executable.tip.bc"
  rm -rf "$SCRATCH_DIR"

  echo $failures
}

# Function to run error tests
run_error_test() {
  testfile="$1"
  failures=0

  ${TIPC} "$testfile" &>/dev/null
  exit_code=$?
  if [ $exit_code -eq 0 ]; then
    echo "Test failure for: $testfile (expected error)" >&2
    rm -f "${testfile}.bc"
    failures=$((failures + 1))
  fi

  echo $failures
}

# Function to run polymorphic type inference tests
run_polytest() {
  testfile="$1"
  base="$(basename "$testfile" .tip)"
  failures=0
  SCRATCH_DIR=$(mktemp -d)

  # Optimized test
  ${TIPC} --pi "$testfile" &>/dev/null
  if [ $? -ne 0 ]; then
    echo "Compilation failed for: $testfile" >&2
    failures=$((failures + 1))
    rm -rf "$SCRATCH_DIR"
    return
  fi

  ${TIPCLANG} -w "$testfile.bc" "${RTLIB}/tip_rtlib.bc" -o "$SCRATCH_DIR/$base" &>/dev/null
  if [ $? -ne 0 ]; then
    echo "Linking failed for: $testfile.bc" >&2
    failures=$((failures + 1))
    rm -f "$testfile.bc"
    rm -rf "$SCRATCH_DIR"
    return
  fi

  "$SCRATCH_DIR/$base" &>/dev/null
  exit_code=$?
  if [ $exit_code -ne 0 ]; then
    echo "Test failure for: $testfile" >&2
    "$SCRATCH_DIR/$base" >&2
    failures=$((failures + 1))
  fi
  rm -f "$testfile.bc"

  # Type checking
  ${TIPC} --pp --pt --pi "$testfile" >"$SCRATCH_DIR/$base.pppt" 2>/dev/null
  diff "$testfile.pppt" "$SCRATCH_DIR/$base.pppt" >"$SCRATCH_DIR/$base.diff"
  if [[ -s "$SCRATCH_DIR/$base.diff" ]]; then
    echo "Test differences for: $testfile" >&2
    cat "$SCRATCH_DIR/$base.diff" >&2
    failures=$((failures + 1))
  fi

  rm -rf "$SCRATCH_DIR"

  echo $failures
}

# Function to run type checking tests
run_typecheck_test() {
  testfile="$1"
  base="$(basename "$testfile")"
  failures=0
  SCRATCH_DIR=$(mktemp -d)

  ${TIPC} -pp -pt "$testfile" >"$SCRATCH_DIR/$base.pppt" 2>/dev/null
  diff "$testfile.pppt" "$SCRATCH_DIR/$base.pppt" >"$SCRATCH_DIR/$base.diff"
  if [[ -s "$SCRATCH_DIR/$base.diff" ]]; then
    echo "Test differences for: $testfile" >&2
    cat "$SCRATCH_DIR/$base.diff" >&2
    failures=$((failures + 1))
  fi

  rm -rf "$SCRATCH_DIR"

  echo $failures
}

# Function to run AST visualizer tests
run_ast_visualizer_test() {
  testfile="$1"
  base="$(basename "$testfile" .tip)"
  failures=0
  SCRATCH_DIR=$(mktemp -d)

  output_graph="$SCRATCH_DIR/${base}.tip.dot"
  expected_output="${testfile}.dot"

  ${TIPC} --pa="$output_graph" "$testfile" &>/dev/null
  diff "$output_graph" "$expected_output" >"$SCRATCH_DIR/${base}.tip.dot.diff"
  if [ -s "$SCRATCH_DIR/${base}.tip.dot.diff" ]; then
    echo "Test differences for: $testfile" >&2
    cat "$SCRATCH_DIR/${base}.tip.dot.diff" >&2
    failures=$((failures + 1))
  fi

  rm -rf "$SCRATCH_DIR"

  echo $failures
}

# Export functions for GNU parallel
export -f run_selftest run_siptest run_iotest run_error_test run_polytest run_typecheck_test run_ast_visualizer_test

# Run self-contained tests in parallel
selftest_files=(selftests/*.tip)
numtests_self=$(( ${#selftest_files[@]} * 2 ))
numfailures_self=$(printf "%s\n" "${selftest_files[@]}" | parallel --no-notice -j "$(nproc)" run_selftest 2>&1 | awk '{sum+=$1} END{print sum}')

# Run SIP tests in parallel
siptest_files=(siptests/*.sip)
numtests_sip=$(( ${#siptest_files[@]} * 2 ))
numfailures_sip=$(printf "%s\n" "${siptest_files[@]}" | parallel --no-notice -j "$(nproc)" run_siptest 2>&1 | awk '{sum+=$1} END{print sum}')

# Run IO tests in parallel
iotest_files=(iotests/*.expected)
numtests_io=${#iotest_files[@]}
numfailures_io=$(printf "%s\n" "${iotest_files[@]}" | parallel --no-notice -j "$(nproc)" run_iotest 2>&1 | awk '{sum+=$1} END{print sum}')

# Run error tests in parallel
error_test_files=(iotests/*error.tip)
numtests_error=${#error_test_files[@]}
numfailures_error=$(printf "%s\n" "${error_test_files[@]}" | parallel --no-notice -j "$(nproc)" run_error_test 2>&1 | awk '{sum+=$1} END{print sum}')

# Run polymorphic type inference tests in parallel
polytest_files=(polytests/*.tip)
numtests_poly=$(( ${#polytest_files[@]} * 2 ))
numfailures_poly=$(printf "%s\n" "${polytest_files[@]}" | parallel --no-notice -j "$(nproc)" run_polytest 2>&1 | awk '{sum+=$1} END{print sum}')

# Run type checking tests in parallel
typecheck_files=(selftests/*.tip siptests/*.sip)
numtests_typecheck=${#typecheck_files[@]}
numfailures_typecheck=$(printf "%s\n" "${typecheck_files[@]}" | parallel --no-notice -j "$(nproc)" run_typecheck_test 2>&1 | awk '{sum+=$1} END{print sum}')

# Run AST visualizer tests in parallel
ast_visualizer_files=(iotests/linkedlist.tip selftests/ptr4.tip)
numtests_ast=${#ast_visualizer_files[@]}
numfailures_ast=$(printf "%s\n" "${ast_visualizer_files[@]}" | parallel --no-notice -j "$(nproc)" run_ast_visualizer_test 2>&1 | awk '{sum+=$1} END{print sum}')

# Total tests and failures
numtests=$((numtests_self + numtests_sip + numtests_io + numtests_error + numtests_poly + numtests_typecheck + numtests_ast))
numfailures=$((numfailures_self + numfailures_sip + numfailures_io + numfailures_error + numfailures_poly + numfailures_typecheck + numfailures_ast))

# Print out the test results
if [ "$numfailures" -eq 0 ]; then
  echo "All $numtests tests passed"
else
  echo "$numfailures/$numtests tests failed"
fi
