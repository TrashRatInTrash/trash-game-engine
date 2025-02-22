#!/bin/bash

# compile and run a test file with the t-vector.c lib file

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <test_file>"
    exit 1
fi

TEST_FILE=$1
IMPL_FILE="t-vector.c"
OUTPUT_EXECUTABLE="test_program.o"

echo "Compiling $TEST_FILE and $IMPL_FILE..."
gcc $TEST_FILE $IMPL_FILE -g -o $OUTPUT_EXECUTABLE

if [ $? -eq 0 ]; then
    echo "Compilation successful. Running $OUTPUT_EXECUTABLE..."
    ./$OUTPUT_EXECUTABLE
else
    echo "Compilation failed. Please check for errors."
    exit 1
fi
