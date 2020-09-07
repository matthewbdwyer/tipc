#!/usr/bin/env bash
declare -r ROOT_DIR=${TRAVIS_BUILD_DIR:-$(git rev-parse --show-toplevel)}

find ${ROOT_DIR} -name '*gcda' -delete
