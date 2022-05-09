CC                       := i686-elf-gcc
CFLAGS                   ?= -Ofast -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wconversion -Wunused-parameter -Wformat-truncation -Wformat-overflow -Werror 
CFLAGS                   := $(CFLAGS) -std=c2x -ffreestanding -c 
CXX                      := i686-elf-g++
CXXFLAGS                 ?= -Ofast -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wconversion -Wunused-parameter -Wformat-truncation -Wformat-overflow -Werror -std=c++2b
CXXFLAGS                 := $(CFLAGS) -c 
AS                       := i686-elf-as
ASFLAGS                  := --msyntax=intel -mnaked-reg
LD                       := i686-elf-gcc # Yes I DO want GCC for some reason?
LDFLAGS                  := -s -T src/linkerScript.ld -ffreestanding -Ofast -nostdlib -lgcc
COMPILE_C_LIB_TO_STATIC  := false

all: dirs bootloader kernel libc link
	@echo "$(shell ./message.sh -m 3)"

dirs:
	mkdir -p bin
	mkdir -p build

bootloader:
	@echo "AS    src/boot.s"
	@$(AS) $(ASFLAGS) src/boot.s -o build/boot.o
	@echo "$(shell ./message.sh -m 0)"

kernel:
	@echo "CC    src/kernel/main.c"
	@$(CC) $(CFLAGS) src/kernel/main.c -o build/kernel.o
	@echo "CC    src/kernel/vga.c"
	@$(CC) $(CFLAGS) src/kernel/vga.c -o build/vga.o
	@echo "$(shell ./message.sh -m 1)"
	
libc:
	@cd src/libc
	@make

link:
	@echo "LD    build/*.o"
	@$(LD) $(LDFLAGS) build/*.o -o bin/kernel.bin
	@echo "$(shell ./message.sh -m 2)"

# toBinary:
#  	# Sleep again to make sure that it has finished linking
# 	@sleep 1
# 	@objcopy -O binary build/bootsect.elf bin/bootsect.bin
# 	@echo "$(shell ./message.sh 4)"

clean:
	@echo "Cleaning..."
	@rm -rf build/*
