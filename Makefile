CC       := g++
CPPC     := gcc
CFLAGS   := -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wconversion -Wunused-parameter -Wformat-truncation -Wformat-overflow -Werror -std=c++2b -Ofast -ffreestanding -fno-exceptions -fno-rtti -c 
AS       := as
ASFLAGS  := --msyntax=intel -mnaked-reg
LNK      := ld
LNKFLAGS := -s -Tsrc/linkerScript.ld -Lsrc/lib -l:cstdlib.a
all: dirs bootloader customLib kernel link toBinary
	@echo "$(shell ./message.sh 5)"

dirs:
	mkdir -p build
	mkdir -p build/cstdlib

bootloader:
	@echo "AS    src/bootloader.s               build/bootloader.o"
	@$(AS) $(ASFLAGS) src/bootloader.s -o build/bootloader.o
	@echo "$(shell ./message.sh 1)"



customLibIO:
	@echo "CPPC    src/cstdlib/io/farpeekl.cpp    build/cstdlib/farpeekl.o"
	@$(CPPC) $(CFLAGS) src/cstdlib/io/farpeekl.cpp -o build/cstdlib/farpeekl.o
	@echo "CPPC    src/cstdlib/io/farpokeb.cpp    build/farpokeb.o"
	@$(CPPC) $(CFLAGS) src/cstdlib/io/farpokeb.cpp -o build/cstdlib/farpokeb.o
	@echo "CPPC    src/cstdlib/io/inb.cpp         build/cstdlib/inb.o"
	@$(CPPC) $(CFLAGS) src/cstdlib/io/inb.cpp -o build/cstdlib/inb.o
	@echo "CPPC    src/cstdlib/io/outb.cpp        build/cstdlib/outb.o"
	@$(CPPC) $(CFLAGS) src/cstdlib/io/outb.cpp -o build/cstdlib/outb.o

customLibMem:
	@echo "CPPC    src/cstdlib/mem/memcpy.cpp     build/cstdlib/memcpy.o"
	@$(CPPC) $(CFLAGS) src/cstdlib/mem/memcpy.cpp -o build/cstdlib/memcpy.o
	@echo "CPPC    src/cstdlib/mem/memset.cpp     build/cstdlib/memset.o"
	@$(CPPC) $(CFLAGS) src/cstdlib/mem/memset.cpp -o build/cstdlib/memset.o

customLibPrint:
	@echo "CPPC    src/cstdlib/print/fputs.cpp    build/cstdlib/fputs.o"
	@$(CPPC) $(CFLAGS) src/cstdlib/print/fputs.cpp -o build/cstdlib/fputs.o
	@echo "CPPC    src/cstdlib/print/putchar.cpp  build/cstdlib/putchar.o"
	@$(CPPC) $(CFLAGS) src/cstdlib/print/putchar.cpp -o build/cstdlib/putchar.o
	@echo "CPPC    src/cstdlib/print/printf.cpp   build/cstdlib/printf.o"
	@$(CPPC) $(CFLAGS) src/cstdlib/print/printf.cpp -o build/cstdlib/printf.o

customLibString:
	@echo "CPPC    src/cstdlib/string/strcat.cpp  build/cstdlib/strcat.o"
	@$(CPPC) $(CFLAGS) src/cstdlib/string/strcat.cpp -o build/cstdlib/strcat.o
	@echo "CPPC    src/cstdlib/string/strcpy.cpp  build/cstdlib/strcpy.o"
	@$(CPPC) $(CFLAGS) src/cstdlib/string/strcpy.cpp -o build/cstdlib/strcpy.o
	@echo "CPPC    src/cstdlib/string/strlen.cpp  build/cstdlib/strlen.o"
	@$(CPPC) $(CFLAGS) src/cstdlib/string/strlen.cpp -o build/cstdlib/strlen.o

customLib: customLibIO customLibMem customLibPrint customLibString
	@echo "$(shell ./message.sh 2)"



kernel:
	@echo "CPPC    src/kernel/kernel.cpp          build/kernel.o"
	@$(CPPC) $(CFLAGS) src/kernel/main.cpp -o build/kernel.o
	@echo "$(shell ./message.sh 3)"


link:
 	# Wait to make sure that the kernel finished compiling (and writing to the disc) before linking
	@sleep 0.25
	@echo "Linking..."
	@ar cr src/lib/cstdlib.a build/cstdlib/*.o
	@$(LNK) $(LNKFLAGS) build/*.o -o build/bootsect.elf
	
toBinary:
 	# Sleep again to make sure that it has finished linking
	@sleep 1
	@objcopy -O binary build/bootsect.elf bin/bootsect.bin
	@echo "$(shell ./message.sh 4)"

clean:
	@echo "Cleaning..."
	@rm -rf build/*
