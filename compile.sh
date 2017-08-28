#!/bin/bash
g++ main.cpp -o main -std=c++1y -Iinclude
if [ $? -ne 0 ]; then
    echo "Compile failed!"
    exit 1
fi




./main
feh out.ppm
