CC                       := gcc
CFLAGS                   := -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wconversion -Wunused-parameter -Wformat-truncation -Wformat-overflow -Werror -std=c2x -g -ffreestanding -fno-exceptions -nostdlib -c 
CXX                      := g++
CXXFLAGS                 := -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wconversion -Wunused-parameter -Wformat-truncation -Wformat-overflow -Werror -std=c++2b -g -ffreestanding -fno-exceptions -fno-rtti -nostdlib -c 
AS                       := as
ASFLAGS                  := --msyntax=intel -mnaked-reg
LNK                      := ld
LNKFLAGS                 := -s -Tsrc/linkerScript.ld -Lsrc/lib -l:libc.so
COMPILE_C_LIB_TO_STATIC  := false
RAINBOW					 := true

all: dirs bootloader kernel link toBinary
	@echo "$(shell ./message.sh 5)"
	@echo "$(shell ./message.sh 5)"

dirs:
	mkdir -p build
	mkdir -p build/cstdlib
	
toBinary:
 	# Sleep again to make sure that it has finished linking
	@sleep 1
	@objcopy -O binary build/bootsect.elf bin/bootsect.bin
	@echo "$(shell ./message.sh 4)"

clean:
	@echo "Cleaning..."
	@rm -rf build/*
