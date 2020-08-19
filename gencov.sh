#!/bin/bash
lcov --capture --directory build -output-file coverage.info
lcov --remove coverage.info '/usr/*' -o coverage.info
lcov --remove coverage.info '*.h' -o coverage.info
lcov --remove coverage.info '*.hpp' -o coverage.info
lcov --remove coverage.info '*/externals/*' -o coverage.info
lcov --remove coverage.info '*/antlr4cpp_generated_src/*' -o coverage.info
genhtml coverage.info -output-directory coverage.out
