#!/bin/bash
# test_script.sh - Comprehensive test for Pixon operations
# Make sure this script has execute permission: chmod +x test_script.sh

set -e  # Exit on error
set -u  # Treat unset variables as errors

# Get the directory where the script resides
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Input files relative to the script directory
INPUT="$SCRIPT_DIR/input.png"
OVERLAY="$SCRIPT_DIR/overlay.png"
OUTPUT_DIR="$SCRIPT_DIR/test_outputs"

mkdir -p "$OUTPUT_DIR"

echo "==== Testing single operations ===="

# Resize
echo "Testing resize..."
pixon "$INPUT" resize 400 300 0 "$OUTPUT_DIR/resize.jpeg"

# Rotate
echo "Testing rotate..."
pixon "$INPUT" rotate 90 "$OUTPUT_DIR/rotate.jpeg"

# Crop
echo "Testing crop..."
pixon "$INPUT" crop 50 50 200 150 "$OUTPUT_DIR/crop.jpeg"

# Blur
echo "Testing blur..."
pixon "$INPUT" blur 2.5 "$OUTPUT_DIR/blur.jpeg"

# Sharpen
echo "Testing sharpen..."
pixon "$INPUT" sharpen "$OUTPUT_DIR/sharpen.jpeg"

# Flip
echo "Testing flip..."
pixon "$INPUT" flip "$OUTPUT_DIR/flip.jpeg"

# Flop
echo "Testing flop..."
pixon "$INPUT" flop "$OUTPUT_DIR/flop.jpeg"

# Composite
echo "Testing composite..."
pixon "$INPUT" composite "$OVERLAY" 50 50 0.7 "$OUTPUT_DIR/composite.jpeg"

# Color adjust
echo "Testing coloradjust..."
pixon "$INPUT" coloradjust 0.1 1.2 1.3 "$OUTPUT_DIR/coloradjust.jpeg"

# Add alpha
echo "Testing addalpha..."
pixon "$INPUT" addalpha "$OUTPUT_DIR/addalpha.png"

# Remove alpha
echo "Testing removealpha..."
pixon "$OUTPUT_DIR/addalpha.png" removealpha "$OUTPUT_DIR/removealpha.jpeg"

# Extend
echo "Testing extend..."
pixon "$INPUT" extend 10 10 20 20 "$OUTPUT_DIR/extend.jpeg"

# Trim (needs alpha)
echo "Testing trim..."
pixon "$OUTPUT_DIR/addalpha.png" trim "$OUTPUT_DIR/trim.png"

echo "==== Testing multiple operations together ===="
pixon "$INPUT" resize 400 300 0 sharpen coloradjust 0.1 1.2 1.3 flip composite "$OVERLAY" 20 20 0.5 "$OUTPUT_DIR/multi_ops.jpeg"

echo "All tests completed. Outputs are in $OUTPUT_DIR/"
