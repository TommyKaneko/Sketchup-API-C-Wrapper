#!/bin/sh

set -eu

SCRIPT_DIR=$(CDPATH= cd -- "$(dirname "$0")" && pwd)
DOXYFILE_PATH=${1:-"$SCRIPT_DIR/DOXYFILE"}
OUTPUT_ROOT=${2:-"$SCRIPT_DIR/.doxygen"}
HTML_DIR="$OUTPUT_ROOT/html"

cd "$SCRIPT_DIR"
rm -rf "$OUTPUT_ROOT"
rm -rf "$SCRIPT_DIR/html"
mkdir -p "$OUTPUT_ROOT"

echo "Generating Doxygen documentation from $DOXYFILE_PATH"
doxygen "$DOXYFILE_PATH"

if [ ! -f "$SCRIPT_DIR/html/index.html" ]; then
    echo "Expected Doxygen output at $SCRIPT_DIR/html/index.html but it was not generated." >&2
    exit 1
fi

rm -rf "$HTML_DIR"
mv "$SCRIPT_DIR/html" "$HTML_DIR"
touch "$HTML_DIR/.nojekyll"

echo "Documentation generated at $HTML_DIR"
