CPPC     := g++
CC       := gcc
CFLAGS   := -Wall -Wextra -Werror -O3 -c
AS       := as
ASFLAGS  := --msyntax=intel -mnaked-reg
LNK      := ld
LNKFLAGS := -s -Tsrc/boot.ld

all: dirs bootloader pci usb print kernel link
	echo "Compiled!"


dirs:
	rm -rf build
	mkdir -p build

bootloader:
	$(AS) $(ASFLAGS) src/setup.s -o build/setup.o

pci:
	$(CPPC) $(CFLAGS) src/kernel/drivers/pci/init.cpp -o build/pciInit.o

usb:
	$(CPPC) $(CFLAGS) src/kernel/drivers/usb/init.cpp -o build/usbInit.o

print:
	$(CPPC) $(CFLAGS) src/kernel/print.cpp -o build/print.o

kernel:
	$(CPPC) $(CFLAGS) src/kernel/main.cpp -o build/kernel.o

link:
	$(LNK) $(LNKFLAGS) build/kernel.o build/setup.o build/pciInit.o build/usbInit.o build/print.o -o bin/bootsect.bin
	
