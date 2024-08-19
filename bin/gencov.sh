#!/bin/sh

# Set ROOT_DIR to GITHUB_WORKSPACE or the top-level Git directory if GITHUB_WORKSPACE is not set
ROOT_DIR=${GITHUB_WORKSPACE:-$(git rev-parse --show-toplevel)}
COV_OUTPUT="${ROOT_DIR}/coverage.info"
HTML_OUTPUT="${ROOT_DIR}/coverage.out"

# Capture coverage data
lcov --capture --directory "${ROOT_DIR}" --no-external --output-file "${COV_OUTPUT}"

# Remove unwanted coverage data
lcov --remove "${COV_OUTPUT}" "${ROOT_DIR}/build/*" -o "${COV_OUTPUT}"
lcov --remove "${COV_OUTPUT}" "${ROOT_DIR}/externals/*" -o "${COV_OUTPUT}"
lcov --remove "${COV_OUTPUT}" '*.h' -o "${COV_OUTPUT}"
lcov --remove "${COV_OUTPUT}" '*.hpp' -o "${COV_OUTPUT}"

# Generate HTML report
genhtml "${COV_OUTPUT}" --output-directory "${HTML_OUTPUT}"

# Print messages to the user
echo "Coverage report has been generated as ${COV_OUTPUT}"
echo "An HTML view of this report is available in ${HTML_OUTPUT}"

