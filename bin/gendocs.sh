#!/bin/sh

# Set ROOT_DIR to GITHUB_WORKSPACE or the top-level Git directory if GITHUB_WORKSPACE is not set
ROOT_DIR=${GITHUB_WORKSPACE:-$(git rev-parse --show-toplevel)}
DOCS_DIR="${ROOT_DIR}/docs"

# Change to the DOCS_DIR directory
cd "${DOCS_DIR}" || exit 1

# Build the docs using Doxygen
doxygen

echo
echo "Documentation has been generated and can be found in ${DOCS_DIR}"
echo "You can view the html by opening ${DOCS_DIR}/html/index.html in a browser"
echo

# Check if coverxygen is installed
if ! python3 -m pip list | grep -q coverxygen; then
  echo "coverxygen is needed to generate a doc coverage report"
  echo "Consider installing it with pip3 install coverxygen"
  exit 0
fi

echo "Running a coverage report on the generated docs"
python3 -m coverxygen \
  --xml-dir xml/ \
  --src-dir ../src \
  --output doc-coverage.info \
  --kind class \
  --scope public

# Generate a JSON summary for a badge
python3 -m coverxygen \
  --xml-dir xml/ \
  --src-dir ../src \
  --output - \
  --format json-summary \
  --kind class \
  --scope public | \
  jq '{"schemaVersion": 1, "label": "doxygen", "color":"informational", "message": "\((.total.coverage_rate*100 | floor))%"}' > html/doccoverage.json

