# Based on kernel/Makefile: For copyright notices, please view that file.
WARN   = -Wall -Wextra -Wstack-protector -Wformat=2 -Wformat-security
FEATURE= -fdiagnostics-color=always -fno-pic -ffreestanding -fstack-protector-all
CFLAGS = $(WARN) $(FEATURE) -nostdlib -I../include -Ofast -g -static -s -std=gnu2x -mcmodel=kernel -march=core2 -mno-3dnow -mno-mmx

override compile_S   := $(shell find . -type f -name '*.S')
override compile_c   := $(shell find . -type f -name '*.c')
compile=$(addprefix ../build/libc/,$(subst ./,,$(subst .c,.o,$(subst .S,.o,$(compile_S) $(compile_c)))))

vpath %.c ./
vpath %.h ../include
outFileName=libk.a # libk.a for kernel library static library.  Not ready for hosted libc yet.

include ../.config

.SUFFIXES: .o .c .S

all: ../lib/$(outFileName)

../lib/$(outFileName): $(compile)
	@mkdir -p ../lib
	@echo "Generating libk archive: $(outFileName)"
	@x86_64-elf-ar cr ../lib/$(outFileName) $(compile)
	@echo "C library ready for use with kernel."

../build/libc/%.o: %.c $(shell find ../include/ -type f)
	@mkdir -p $(@D)
	@echo "CC    $< => $(subst ../build/libc/,,$@)"
	@x86_64-elf-gcc $(CFLAGS) -mno-red-zone -c $< -o $@


clean:
	@rm -f $(compile) ../build/libc/*.a || true

softclean:
	@rm -f $(compile) || true
