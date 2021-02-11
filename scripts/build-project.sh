#!/usr/bin/env bash

set -euo pipefail

# Validate input parameters
if [[ $# -lt 2 ]]; then
    echo "Usage: ./build-project.sh [core] [board]"
    echo "Example: ./build-project.sh avr uno"
    exit 1
fi
CORE=$1
BOARD=$2

# Build the code for the input Arduino boards
arduino-cli version
arduino-cli core update-index
arduino-cli core install arduino:$CORE
arduino-cli core list
# install libraries
arduino-cli lib install "PID"
arduino-cli lib install "TSIC"
arduino-cli lib install "SSD1306Ascii"
arduino-cli lib install "MAX6675 library"
arduino-cli compile --fqbn arduino:$CORE:$BOARD CoffeeMachine/CoffeeMachine.ino
