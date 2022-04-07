#!/bin/sh
rm -rf build
mkdir -p bin
mkdir -p build
as --msyntax=intel -mnaked-reg src/setup.s -o build/setup.o
g++ -Wall -Wextra -Werror -c src/kernel/main.cpp -O3 -o build/kernel.o
g++ -Wall -Wextra -Werror -c src/kernel/drivers/pci/init.cpp -O3 -o build/pciInit.o
g++ -Wall -Wextra -Werror -c src/kernel/drivers/usb/init.cpp -O3 -o build/usbInit.o
g++ -Wall -Wextra -Werror -c src/kernel/print.cpp -O3 -o build/print.o
ld -s -Tsrc/boot.ld -o bin/bootsect.bin build/kernel.o build/setup.o build/pciInit.o build/usbInit.o build/print.o
