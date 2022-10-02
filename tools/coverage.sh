#!/bin/bash

g++ --coverage google_tests.cpp -o google_tests -lgtest -lpthread
./google_tests
lcov -t "google_tests" -o google_tests.info -c -d . #--rc lcov_branch_coverage=1
genhtml -o report google_tests.info #--rc lcov_branch_coverage=1