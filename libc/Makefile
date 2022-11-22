# Based on kernel/Makefile: For copyright notices, please view that file.
WARN   = -Wall -Wextra -Wstack-protector -Wformat=2 -Wformat-security
FEATURE= -fdiagnostics-color=always -fno-pic -ffreestanding -fstack-protector-all
CFLAGS = $(WARN) $(FEATURE) -nostdlib -I../include -g -static -s -std=gnu2x -mcmodel=kernel -march=core2 -mno-3dnow -mno-mmx

compile=\
stdio/puts.o \
stdio/putchar.o \
stdio/printf.o \
stdlib/atoi.o \
stdlib/itoa.o \
stdlib/utoa.o \
string/strlen.o \
string/memset.o \
string/memcmp.o \
string/memcpy.o \
string/memmove.o \
string/strcat.o \
string/strcpy.o \
string/strcmp.o \
string/strstr.o \
custom/putcAt.o \
custom/putsNoTerminator.o \
custom/padNumTo.o

link=\
../build/libc/stdio/puts.o \
../build/libc/stdio/putchar.o \
../build/libc/stdio/printf.o \
../build/libc/stdlib/atoi.o \
../build/libc/stdlib/itoa.o \
../build/libc/stdlib/utoa.o \
../build/libc/string/strlen.o \
../build/libc/string/memset.o \
../build/libc/string/memcmp.o \
../build/libc/string/memcpy.o \
../build/libc/string/memmove.o \
../build/libc/string/strcat.o \
../build/libc/string/strcpy.o \
../build/libc/string/strcmp.o \
../build/libc/string/strstr.o \
../build/libc/custom/putcAt.o \
../build/libc/custom/putsNoTerminator.o \
../build/libc/custom/padNumTo.o

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
