#!/bin/sh
rm -rf build
mkdir -p bin
mkdir -p build
as --msyntax=intel -mnaked-reg src/setup.s -o build/setup.o
g++ -Wall -Wextra -Werror -c src/kernel/main.cpp -O3 -o build/kernel.o
ld -s -Tsrc/boot.ld -o bin/bootsect.bin build/kernel.o build/setup.o
