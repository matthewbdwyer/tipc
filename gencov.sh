#!/bin/bash
lcov --capture --directory build/src -output-file /tmp/tipc.coverage.info
lcov --remove /tmp/tipc.coverage.info '/usr/include/*' -o /tmp/tipc.coverage.info
lcov --remove /tmp/tipc.coverage.info '*.h' -o /tmp/tipc.coverage.info
lcov --remove /tmp/tipc.coverage.info '*.def' -o /tmp/tipc.coverage.info
genhtml /tmp/tipc.coverage.info -output-directory coverage.out
