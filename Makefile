CPPC     := g++
CC       := gcc
CFLAGS   := -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wconversion -Wunused-parameter -Wformat-truncation -Wformat-overflow -Werror -std=c++2b -Ofast -ffreestanding -fno-exceptions -fno-rtti -c 
AS       := as
ASFLAGS  := --msyntax=intel -mnaked-reg
LNK      := ld
LNKFLAGS := -s -Tsrc/linkerScript.ld
all: dirs bootloader customLib kernel link toBinary
	@echo "$(shell ./message.sh 5)"

dirs:
	mkdir -p build

bootloader:
	@echo "AS    src/bootloader.s               build/bootloader.o"
	@$(AS) $(ASFLAGS) src/bootloader.s -o build/bootloader.o
	@echo "$(shell ./message.sh 1)"



customLibIO:
	@echo "CC    src/cstdlib/io/farpeekl.cpp    build/farpeekl.o"
	@$(CC) $(CFLAGS) src/cstdlib/io/farpeekl.cpp -o build/farpeekl.o
	@echo "CC    src/cstdlib/io/farpokeb.cpp    build/farpokeb.o"
	@$(CC) $(CFLAGS) src/cstdlib/io/farpokeb.cpp -o build/farpokeb.o
	@echo "CC    src/cstdlib/io/inb.cpp         build/inb.o"
	@$(CC) $(CFLAGS) src/cstdlib/io/inb.cpp -o build/inb.o
	@echo "CC    src/cstdlib/io/outb.cpp        build/outb.o"
	@$(CC) $(CFLAGS) src/cstdlib/io/outb.cpp -o build/outb.o

customLibMem:
	@echo "CC    src/cstdlib/mem/memcpy.cpp     build/memcpy.o"
	@$(CC) $(CFLAGS) src/cstdlib/mem/memcpy.cpp -o build/memcpy.o
	@echo "CC    src/cstdlib/mem/memset.cpp     build/memset.o"
	@$(CC) $(CFLAGS) src/cstdlib/mem/memset.cpp -o build/memset.o

customLibPrint:
	@echo "CC    src/cstdlib/print/fputs.cpp    build/fputs.o"
	@$(CC) $(CFLAGS) src/cstdlib/print/fputs.cpp -o build/fputs.o
	@echo "CC    src/cstdlib/print/putchar.cpp  build/putchar.o"
	@$(CC) $(CFLAGS) src/cstdlib/print/putchar.cpp -o build/putchar.o
	@echo "CC    src/cstdlib/print/printf.cpp   build/printf.o"
	@$(CC) $(CFLAGS) src/cstdlib/print/printf.cpp -o build/printf.o

customLibString:
	@echo "CC    src/cstdlib/string/strncat.cpp build/strncat.o"
	@$(CC) $(CFLAGS) src/cstdlib/string/strcat.cpp -o build/strcat.o
	@echo "CC    src/cstdlib/string/strcpy.cpp  build/strcpy.o"
	@$(CC) $(CFLAGS) src/cstdlib/string/strcpy.cpp -o build/strcpy.o
	@echo "CC    src/cstdlib/string/strlen.cpp  build/strlen.o"
	@$(CC) $(CFLAGS) src/cstdlib/string/strlen.cpp -o build/strlen.o

customLib: customLibIO customLibMem customLibPrint customLibString
	@echo "$(shell ./message.sh 2)"



kernel:
	@echo "CC    src/kernel/kernel.cpp          build/kernel.o"
	@$(CPPC) $(CFLAGS) src/kernel/main.cpp -o build/kernel.o
	@echo "$(shell ./message.sh 3)"


link:
 	# Wait to make sure that the kernel finished compiling (and writing to the disc) before linking
	@sleep 0.25
	@echo "Linking..."
	@$(LNK) $(LNKFLAGS) build/*.o -o build/bootsect.elf
	
toBinary:
 	# Sleep again to make sure that it has finished linking
	@sleep 1
	@objcopy -O binary build/bootsect.elf bin/bootsect.bin
	@echo "$(shell ./message.sh 4)"
