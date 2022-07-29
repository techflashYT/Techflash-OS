# Based on kernel/Makefile: For copyright notices, please view that file.

CFLAGS = -Wall -Wextra -fdiagnostics-color=always -fno-pic -ffreestanding -fno-stack-protector -nostdlib -I../include -Ofast -static -s -std=gnu2x -mcmodel=kernel -mgeneral-regs-only -mno-3dnow -mno-mmx -mno-sse -mno-sse2 -mno-avx

compile=\
stdio/puts.o \
stdio/putchar.o \
stdio/printf.o \
stdlib/itoa.o \
string/strlen.o \
string/memset.o \
custom/putcAt.o \
custom/putsNoTerminator.o

link=\
../build/libc/stdio/puts.o \
../build/libc/stdio/putchar.o \
../build/libc/stdio/printf.o \
../build/libc/stdlib/itoa.o \
../build/libc/string/strlen.o \
../build/libc/string/memset.o \
../build/libc/custom/putcAt.o \
../build/libc/custom/putsNoTerminator.o

outFileName=libk.a # libk.a for kernel library static library.  Not ready for hosted libc yet.

include ../.config

.SUFFIXES: .o .c .S

all: $(compile)
	@echo "Generating libk archive: $(outFileName)"
	@x86_64-elf-ar cr ../lib/$(outFileName) $(link)
	@echo "C library ready for use with kernel."

.c.o:
	@echo "CC    $(subst .o,.c,$@) => $@"
	@x86_64-elf-gcc $(CFLAGS) -mno-red-zone -c $(subst .o,.c,$@) -o ../build/libc/$@

clean:
	@rm -f $(link) ../build/libc/*.a || true

softclean:
	@rm -f $(link) || true
