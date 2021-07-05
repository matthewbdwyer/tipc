#!/usr/bin/env bash
declare -r ROOT_DIR=${GITHUB_WORKSPACE:-$(git rev-parse --show-toplevel)}

find ${ROOT_DIR} -name '*gcda' -delete
