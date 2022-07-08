# Based on kernel/Makefile: For copyright notices, please view that file.

CFLAGS = -Wall -Wextra -fpic -ffreestanding -fno-stack-protector -nostdlib -I../include -Ofast -static -s

compile=\
stdio/puts.o

link=\
../build/libc/stdio/puts.o

outFileName=libk.a # libk.a for kernel library static library.  Not ready for hosted libc yet.

include ../.config

.SUFFIXES: .o .c .S .S64

all: $(compile)
	@echo "Generating libk archive: $(outFileName)"
	@x86_64-elf-ar cr ../lib/$(outFileName) $(link)
	@echo "C library ready for use with kernel."

.c.o:
	@echo "CC    $(subst .o,.c,$@) => $@"
	@x86_64-elf-gcc $(CFLAGS) -mno-red-zone -c $(subst .o,.c,$@) -o ../build/libc/$@

clean:
	@rm -f ../build/libc/*.o ../build/libc/*.a || true

softclean:
	@rm -f ../build/libc/*.o || true
