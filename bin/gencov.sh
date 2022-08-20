#!/usr/bin/env bash
declare -r ROOT_DIR=${GITHUB_WORKSPACE:-$(git rev-parse --show-toplevel)}
declare -r COV_OUTPUT=${ROOT_DIR}/coverage.info
declare -r HTML_OUTPUT=${ROOT_DIR}/coverage.out

lcov --capture --directory ${ROOT_DIR} --no-external -output-file ${COV_OUTPUT}
lcov --remove coverage.info ${ROOT_DIR}'/build/*' -o ${COV_OUTPUT}
lcov --remove coverage.info ${ROOT_DIR}'/externals/*' -o ${COV_OUTPUT}
lcov --remove coverage.info ${ROOT_DIR}'/test/*' -o ${COV_OUTPUT}
lcov --remove coverage.info '*.h' -o coverage.info
lcov --remove coverage.info '*.hpp' -o coverage.info
genhtml ${COV_OUTPUT} -output-directory ${HTML_OUTPUT}

echo Coverage report has been generated as ${COV_OUTPUT}
echo An HTML view of this report is available in ${HTML_OUTPUT}
