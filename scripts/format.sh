#!/bin/bash

# Get the path of the directory where the script is located
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# Format source files
clang-format -i "$SCRIPT_DIR/../src/"*.cc
clang-format -i "$SCRIPT_DIR/../src/"*.hh

echo "All files formatted"
