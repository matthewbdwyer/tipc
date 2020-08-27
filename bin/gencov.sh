#!/usr/bin/env bash
declare -r ROOT_DIR=${TRAVIS_BUILD_DIR:-$(git rev-parse --show-toplevel)}

pushd $ROOT_DIR
lcov --capture --directory build -output-file coverage.info
lcov --remove coverage.info '/usr/*' -o coverage.info
lcov --remove coverage.info '/Applications/*' -o coverage.info
lcov --remove coverage.info '*.h' -o coverage.info
lcov --remove coverage.info '*.hpp' -o coverage.info
lcov --remove coverage.info '*/externals/*' -o coverage.info
lcov --remove coverage.info '*/antlr4cpp_generated_src/*' -o coverage.info
genhtml coverage.info -output-directory coverage.out
if [ -n "$CI" ]; then
  bash <(curl -s https://codecov.io/bash) \
    -f coverage.info \
    -F codecoverage || 
    echo "Codecov did not collect coverage reports"
fi
popd

echo Coverage report has been generated as coverage.info
echo An HTML view of this report is available in coverage.out
