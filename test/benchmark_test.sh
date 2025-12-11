#!/bin/bash
# benchmark_test.sh - Benchmark all Pixon operations
# Make sure this script has execute permission: chmod +x benchmark_test.sh

set -e  # Exit on error
set -u  # Treat unset variables as errors

# Get the directory where the script resides
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Input files relative to the script directory
INPUT="$SCRIPT_DIR/input.png"
OVERLAY="$SCRIPT_DIR/overlay.png"
OUTPUT_DIR="$SCRIPT_DIR/benchmark_outputs"

mkdir -p "$OUTPUT_DIR"

echo "==== Benchmarking single operations ===="

benchmark() {
    local desc=$1
    shift
    echo "Testing $desc..."
    start=$(date +%s.%N)
    "$@"
    end=$(date +%s.%N)
    elapsed=$(echo "$end - $start" | bc)
    printf "Operation %s took %.3f seconds\n\n" "$desc" "$elapsed"
}

# Single operations
benchmark "resize" pixon "$INPUT" resize 400 300 0 "$OUTPUT_DIR/resize.jpeg"
benchmark "rotate" pixon "$INPUT" rotate 90 "$OUTPUT_DIR/rotate.jpeg"
benchmark "crop" pixon "$INPUT" crop 50 50 200 150 "$OUTPUT_DIR/crop.jpeg"
benchmark "blur" pixon "$INPUT" blur 2.5 "$OUTPUT_DIR/blur.jpeg"
benchmark "sharpen" pixon "$INPUT" sharpen "$OUTPUT_DIR/sharpen.jpeg"
benchmark "flip" pixon "$INPUT" flip "$OUTPUT_DIR/flip.jpeg"
benchmark "flop" pixon "$INPUT" flop "$OUTPUT_DIR/flop.jpeg"
benchmark "composite" pixon "$INPUT" composite "$OVERLAY" 50 50 0.7 "$OUTPUT_DIR/composite.jpeg"
benchmark "coloradjust" pixon "$INPUT" coloradjust 0.1 1.2 1.3 "$OUTPUT_DIR/coloradjust.jpeg"
benchmark "addalpha" pixon "$INPUT" addalpha "$OUTPUT_DIR/addalpha.png"
benchmark "removealpha" pixon "$OUTPUT_DIR/addalpha.png" removealpha "$OUTPUT_DIR/removealpha.jpeg"
benchmark "extend" pixon "$INPUT" extend 10 10 20 20 "$OUTPUT_DIR/extend.jpeg"
benchmark "trim" pixon "$OUTPUT_DIR/addalpha.png" trim "$OUTPUT_DIR/trim.png"

echo "==== Benchmarking multiple operations together ===="
benchmark "multi_ops" pixon "$INPUT" resize 400 300 0 sharpen coloradjust 0.1 1.2 1.3 flip composite "$OVERLAY" 20 20 0.5 "$OUTPUT_DIR/multi_ops.jpeg"

echo "All benchmarks completed. Outputs are in $OUTPUT_DIR/"



# cd pixon/test
# chmod +x benchmark_test.sh
# ./benchmark_test.sh
