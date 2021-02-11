#!/usr/bin/env bash

set -euo pipefail

# Build the automated test suite
mkdir -p build
cd build
cmake ..
make

# Run the test
cd tests
export GTEST_COLOR=1
ctest --verbose
