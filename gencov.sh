#!/bin/bash
lcov --capture --directory build -output-file coverage.info
lcov --remove coverage.info '/usr/include/*' -o coverage.info
lcov --remove coverage.info '/usr/lib/*' -o coverage.info
lcov --remove coverage.info '*.h' -o coverage.info
lcov --remove coverage.info '*.hpp' -o coverage.info
genhtml coverage.info -output-directory coverage.out
