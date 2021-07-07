#!/usr/bin/env bash
declare -r ROOT_DIR=${GITHUB_WORKSPACE:-$(git rev-parse --show-toplevel)}
declare -r DOCS_DIR=${ROOT_DIR}/docs

pushd $DOCS_DIR

# Build the docs.
doxygen

echo 
echo Documentation has been generated and can be found in ${DOCS_DIR}
echo You can view the html by opening ${DOCS_DIR}/html/index.html in a browser
echo 

python3 -m pip list | grep coverxygen
if [ $? -ne 0 ]; then
  echo coverxygen is needed to generate a doc coverage report
  echo consider installing with pip3 install coverxygen
  exit 0
fi

echo Running a coverage report on the generated docs
python3 -m coverxygen \
  --xml-dir xml/ \
  --src-dir ../src \
  --output doc-coverage.info \
  --kind class \
  --scope public

# Badges and flags are flaky in codecov. Let's dump some json and 
# let shields build our badge. 
python3 -m coverxygen \
  --xml-dir xml/ \
  --src-dir ../src \
  --output - \
  --format json-summary \
  --kind class \
  --scope public | 
  jq '{"schemaVersion": 1,  "label": "doxygen","color":"informational","message": "\((.total.coverage_rate*100 | floor))%"}' > html/doccoverage.json
