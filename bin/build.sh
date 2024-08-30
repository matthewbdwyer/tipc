#!/bin/bash
set -e

# Set ROOT_DIR to TIPDIR or the top-level Git directory if TIPDIR is not set
ROOT_DIR=${TIPDIR:-$(git rev-parse --show-toplevel)}
TIPC="${ROOT_DIR}/build/src/tipc"
RTLIB="${ROOT_DIR}/rtlib"

# Check if TIPCLANG environment variable is set
if [ -z "${TIPCLANG}" ]; then
  echo "error: TIPCLANG env var must be set"
  exit 1
fi

# Check if the tipc executable exists
if [ ! -f "${TIPC}" ]; then
  echo "error: tipc was not found"
  exit 1
fi

# Check if the tip_rtlib.bc file exists
if [ ! -f "${RTLIB}/tip_rtlib.bc" ]; then
  echo "error: tip_rtlib.bc was not found"
  exit 1
fi

set -- "$@"

# Execute tipc with the provided arguments
${TIPC} "$@"

# Only perform link step if bitcode has been generated
case "$*" in
  *--help*|*--asm*)
    # Do nothing if --help or --asm is present
    ;;
  *)
    # Perform the linking step
    ${TIPCLANG} -w "${@:$#}.bc" "${RTLIB}/tip_rtlib.bc" -o "$(basename "${@:$#}" .tip)"
    ;;
esac