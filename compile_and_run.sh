#!/bin/bash



# compile and run a test file with lib file

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <test_file> <output_file>"
    exit 1
fi

TEST_FILE=$1
OUTPUT_EXECUTABLE=$2

echo "Compiling $TEST_FILE ... "
gcc $TEST_FILE lib/T-engine.c lib/t-lib.c lib/t-vector.c -Ilib -I/usr/include/SDL2 -lSDL2 -lSDL2_ttf -lm -g -o $OUTPUT_EXECUTABLE

if [ $? -eq 0 ]; then
    echo "Compilation successful Running $OUTPUT_EXECUTABLE..."
    ./$OUTPUT_EXECUTABLE
else
    echo "Compilation failed"
    exit 1
fi



# echo "Compiling demo.c and library files..."
# gcc demo.c lib/T-engine.c lib/t-lib.c lib/t-vector.c -Ilib -I/usr/include/SDL2 -lSDL2 -lSDL2_ttf -o demo.o -g

# if [ $? -eq 0 ]; then
#     echo "Compilation successful. Running demo..."
#     ./demo.o
# else
#     echo "Compilation failed. Please check for errors."
#     exit 1
# fi
