#!/usr/bin/env bash

if [[ $# -lt 2 ]]; then
    echo "Usage: ./build-project.sh [core] [board]"
    echo "Example: ./build-project.sh avr uno"
    exit 1
fi

CORE=$1
BOARD=$2

arduino-cli version
arduino-cli core update-index
arduino-cli core install arduino:$CORE
arduino-cli core list
# install libraries
arduino-cli lib install "PID"
arduino-cli lib install "TSIC"
arduino-cli compile --fqbn arduino:$CORE:$BOARD GaggiaPIDController/GaggiaPIDController.ino
