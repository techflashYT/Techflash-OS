CC                       := gcc
CFLAGS                   := -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wconversion -Wunused-parameter -Wformat-truncation -Wformat-overflow -Werror -std=c2x -g -ffreestanding -fno-exceptions -c 
CXX                      := g++
CXXFLAGS                 := -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wconversion -Wunused-parameter -Wformat-truncation -Wformat-overflow -Werror -std=c++2b -g -ffreestanding -fno-exceptions -fno-rtti -c 
AS                       := as
ASFLAGS                  := --msyntax=intel -mnaked-reg
LNK                      := ld
LNKFLAGS                 := -s -Tsrc/linkerScript.ld
COMPILE_C_LIB_TO_STATIC  := false
RAINBOW					 := true

all: dirs bootloader customLib kernel link toBinary
	@echo "$(shell ./message.sh 5)"

dirs:
	mkdir -p build
	mkdir -p build/cstdlib

bootloader:
	@echo "AS    src/bootloader.s                    build/bootloader.o"
	@$(AS) $(ASFLAGS) src/bootloader.s -o build/bootloader.o
	@echo "$(shell ./message.sh 1)"



customLibIO:
	@echo "CC    src/cstdlib/io/farpeekl.c           build/cstdlib/farpeekl.o"
	@$(CC) $(CFLAGS) src/cstdlib/io/farpeekl.c -o build/cstdlib/farpeekl.o
	@echo "CC    src/cstdlib/io/farpokeb.c           build/farpokeb.o"
	@$(CC) $(CFLAGS) src/cstdlib/io/farpokeb.c -o build/cstdlib/farpokeb.o
	@echo "CC    src/cstdlib/io/inb.c                build/cstdlib/inb.o"
	@$(CC) $(CFLAGS) src/cstdlib/io/inb.c -o build/cstdlib/inb.o
	@echo "CC    src/cstdlib/io/outb.cpp             build/cstdlib/outb.o"
	@$(CC) $(CFLAGS) src/cstdlib/io/outb.c -o build/cstdlib/outb.o

customLibMem:
	@echo "CC    src/cstdlib/mem/klmalloc.c          build/cstdlib/klmalloc.o"
	@$(CC) $(CFLAGS) src/cstdlib/mem/klmalloc.c -o build/cstdlib/klmalloc.o
	@echo "CC    src/cstdlib/mem/malloc.c            build/cstdlib/malloc.o"
	@$(CC) $(CFLAGS) src/cstdlib/mem/malloc.c -o build/cstdlib/malloc.o
	@echo "CC    src/cstdlib/mem/memcpy.c            build/cstdlib/memcpy.o"
	@$(CC) $(CFLAGS) src/cstdlib/mem/memcpy.c -o build/cstdlib/memcpy.o
	@echo "CC    src/cstdlib/mem/memset.c            build/cstdlib/memset.o"
	@$(CC) $(CFLAGS) src/cstdlib/mem/memset.c -o build/cstdlib/memset.o
	@echo "CC    src/cstdlib/mem/sbrk.c              build/cstdlib/sbrk.o"
	@$(CC) $(CFLAGS) src/cstdlib/mem/sbrk.c -o build/cstdlib/sbrk.o
	@echo "CC    src/cstdlib/mem/spinlock.c          build/cstdlib/spinlock.o"
	@$(CC) $(CFLAGS) src/cstdlib/mem/spinlock.c -o build/cstdlib/spinlock.o
	@echo "CC    src/cstdlib/mem/switchtask.c        build/cstdlib/switchtask.o"
	@$(CC) $(CFLAGS) src/cstdlib/mem/switchtask.c -o build/cstdlib/switchtask.o

customLibPrint:
	@echo "CXX   src/cstdlib/print/fputs.cpp         build/cstdlib/fputs.o"
	@$(CXX) $(CXXFLAGS) src/cstdlib/print/fputs.cpp -o build/cstdlib/fputs.o
	@echo "CC    src/cstdlib/print/putchar.c         build/cstdlib/putchar.o"
	@$(CC) $(CFLAGS) src/cstdlib/print/putchar.c -o build/cstdlib/putchar.o
	@echo "CXX   src/cstdlib/print/putc.cpp          build/cstdlib/putc.o"
	@$(CXX) $(CXXFLAGS) src/cstdlib/print/putc.cpp -o build/cstdlib/putc.o
	@echo "CC    src/cstdlib/print/printf.c          build/cstdlib/printf.o"
	@$(CC) $(CFLAGS) src/cstdlib/print/printf.c -o build/cstdlib/printf.o
	@echo "CC    src/cstdlib/print/vasprintf.c       build/cstdlib/vasprintf.o"
	@$(CC) $(CFLAGS) src/cstdlib/print/vasprintf.c -o build/cstdlib/vasprintf.o

customLibString:
	@echo "CC    src/cstdlib/string/strcat.c         build/cstdlib/strcat.o"
	@$(CC) $(CFLAGS) src/cstdlib/string/strcat.c -o build/cstdlib/strcat.o
	@echo "CC    src/cstdlib/string/strcpy.c         build/cstdlib/strcpy.o"
	@$(CC) $(CFLAGS) src/cstdlib/string/strcpy.c -o build/cstdlib/strcpy.o
	@echo "CC    src/cstdlib/string/strlen.c         build/cstdlib/strlen.o"
	@$(CC) $(CFLAGS) src/cstdlib/string/strlen.c -o build/cstdlib/strlen.o

customLib: customLibIO customLibMem customLibPrint customLibString
	@echo "$(shell ./message.sh 2)"



kernel:
	@echo "CXX   src/kernel/kernel.cpp                build/kernel.o"
	@$(CXX) $(CXXFLAGS) src/kernel/main.cpp -o build/kernel.o
	@echo "CC    src/kernel/kernelPanic.c             build/kernelPanic.o"
	@$(CC) $(CFLAGS) src/kernel/kernelPanic.c -o build/kernelPanic.o
	@echo "CC    src/kernel/drivers/cpu/x86_64/GDT.c  build/GDT.o"
	@echo "$(shell ./message.sh 3)"


link:
 	# Wait to make sure that the kernel finished compiling (and writing to the disc) incase before linking
	@sleep 0.25
	@echo "Linking..."
ifeq ($(COMPILE_C_LIB_TO_STATIC),true)
	@echo "$(shell ./message.sh 6)"
	@ar cr src/lib/cstdlib.a build/cstdlib/*.o
	@echo "$(shell ./message.sh e)"
	@$(LNK) $(LNKFLAGS) -Lsrc/lib -l:cstdlib.a build/*.o -o build/bootsect.elf
else
	@$(LNK) $(LNKFLAGS) build/*.o build/cstdlib/*.o -o build/bootsect.elf
endif
	
toBinary:
 	# Sleep again to make sure that it has finished linking
	@sleep 1
	@objcopy -O binary build/bootsect.elf bin/bootsect.bin
	@echo "$(shell ./message.sh 4)"

clean:
	@echo "Cleaning..."
	@rm -rf build/*
