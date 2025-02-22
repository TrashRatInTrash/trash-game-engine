#!/bin/bash

echo "Compiling demo.c and library files..."
gcc demo.c lib/T-engine.c lib/t-lib.c lib/t-vector.c -Ilib -I/usr/include/SDL2 -lSDL2 -lSDL2_ttf -o demo.o -g

if [ $? -eq 0 ]; then
    echo "Compilation successful. Running demo..."
    ./demo.o
else
    echo "Compilation failed. Please check for errors."
    exit 1
fi
