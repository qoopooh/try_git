#!/usr/bin/env bash
set -euo pipefail  # Exit on error, undefined variable, or error in pipeline

FQBN="esp32:esp32:nologo_esp32c3_super_mini"
: ${PORT:="/dev/ttyACM0"}  # arduino-cli board list
BAUD="115200"
SKETCH_DIR="$(dirname "$(realpath "$0")")"

echo "=== Compile ==="
echo "Compiling sketch in $SKETCH_DIR for board $FQBN..."
START_TIME=$(date +%s)
arduino-cli compile --fqbn "$FQBN" "$SKETCH_DIR"
END_TIME=$(date +%s)
ELAPSED=$((END_TIME - START_TIME))
echo "Compilation completed in ${ELAPSED}s"

echo "=== Upload ==="
arduino-cli upload --fqbn "$FQBN" --port "$PORT" "$SKETCH_DIR"

echo "=== Monitor (Ctrl+C to exit) ==="
arduino-cli monitor --port "$PORT" --config "baudrate=$BAUD"
