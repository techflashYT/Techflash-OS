CC                       := i686-elf-gcc
CFLAGS                   := -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wconversion -Wunused-parameter -Wformat-truncation -Wformat-overflow -Werror -std=gnu99 -ffreestanding -g -c 
CXX                      := i686-elf-g++
CXXFLAGS                 := -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wconversion -Wunused-parameter -Wformat-truncation -Wformat-overflow -Werror -std=c++2b -g -c 
AS                       := i686-elf-as
ASFLAGS                  := --msyntax=intel -mnaked-reg
LD                       := i686-elf-ld
LDFLAGS                  := -s
COMPILE_C_LIB_TO_STATIC  := false
RAINBOW					 := true

all: dirs bootloader kernel link
	@echo "$(shell ./message.sh 5)"
	@echo "$(shell ./message.sh 5)"

dirs:
	mkdir -p bin
	mkdir -p build

bootloader:
	@echo "AS    src/boot.s"
	@$(AS) $(ASFLAGS) src/boot.s -o boot.o

kernel:
	@echo "CC    src/kernel/main.c"
	@$(CC) $(CFLAGS) src/kernel/main.c -o kernel.o
	@echo "CC    src/kernel/vga.c"
	@$(CC) $(CFLAGS) src/kernel/vga.c -o vga.o

link:
	$(LD) $(LDFLAGS) build/*.o -o bin/kernel.bin

# toBinary:
#  	# Sleep again to make sure that it has finished linking
# 	@sleep 1
# 	@objcopy -O binary build/bootsect.elf bin/bootsect.bin
# 	@echo "$(shell ./message.sh 4)"

clean:
	@echo "Cleaning..."
	@rm -rf build/*
