# Original version Copyright 2017 - 2021 bzt (bztsrc@git.lab)
# as part of the 'bootboot' repository on GitLab
# Original File is availible at https://gitlab.com/bztsrc/bootboot/-/blob/master/mykernel/c/Makefile
CPUFLAGS   = -march=core2 -mno-3dnow -mno-mmx
FEATURES   = -fbuiltin -fdiagnostics-color=always -fno-pic -ffreestanding -fstack-protector-all -flto
OPTIMIZE   = -Ofast # NORMAL
# OPTIMIZE   = -O2 # STABLE
# OPTIMIZE   = -O0 # DEBUG
WARNINGS   = -Wall -Wextra -Wunused -Wunreachable-code -Wno-uninitialized -Wno-strict-aliasing -Werror
CFLAGS     = $(WARNINGS) -g -nostdlib -I../include -imacros misc/defines.h -include kernel/kernel.h -include kernel/log.h -std=gnu2x -mcmodel=kernel -fno-omit-frame-pointer $(CPUFLAGS) $(OPTIMIZE) $(FEATURES)
LDFLAGS    = -fdiagnostics-color=always -nostdlib -T linkerScript.ld -static -L../lib -e _start
LIBS       = -lk
STRIPFLAGS = -s -K mmio -K fb -K bootboot -K environment -K initstack
INCLUDES   = $(shell find -O3 ../include/ -type f)
SHELL=/bin/bash
vpath %.c ./
vpath %.h ../include
include compLnk.mk

include ../.config
.SUFFIXES: .o .c .S

all: ../bin/tfos_kernel.elf
../bin/tfos_kernel.elf: ../build/tfos_kernel_nosyms.elf ../build/symbols.o
	@mkdir -p ../bin
	@echo "CCLD  $(subst ../build/kernel/,,$(compile)) symbols.o => tfos_kernel.elf"
	@$(CC) $(LDFLAGS) $(compile) ../build/kernel/testELF.o ../build/symbols.o $(LIBS) -o ../bin/tfos_kernel.elf  2> >(../util/rederr.sh >&2);exit $$?
		
#@echo "Stripping Debug Symbols..."
#@x86_64-elf-strip $(STRIPFLAGS) ../bin/tfos_kernel.elf
	@echo "Making sysroot..."
	@../util/sysroot.sh doIt
../build/tfos_kernel_nosyms.elf: ../build/kernel/testELF.o $(compile) ../build/kernel/symTableDummyDat.o
	@echo "CCLD  $(subst ../build/kernel/,,$(compile)) => tfos_kernel_nosyms.elf"
	@$(CC) $(LDFLAGS) $(compile) ../build/kernel/testELF.o ../build/kernel/symTableDummyDat.o $(LIBS) -o ../build/tfos_kernel_nosyms.elf  2> >(../util/rederr.sh >&2);exit $$?

../build/symbols.o: ../build/tfos_kernel_nosyms.elf
	@cd ../ && mkdir -p bin && util/symbols.sh && cd kernel

../build/kernel/symTableDummyDat.o: ../util/symTableDummyDat.c
	@echo "CC    $< => $(subst ../build/kernel/,,$@)"
	@$(CC) $(CFLAGS) -c $< -o $@

../build/kernel/testELF.o: ../test
	@cd ../ && mkdir -p build/kernel && $(LD) -r -b binary test -o build/kernel/testELF.o && cd kernel

../build/kernel/%.o: %.c $(INCLUDES)
	@mkdir -p $(@D)
	@echo "CC    $< => $(subst ../build/kernel/,,$@)"
	@x86_64-elf-gcc $(CFLAGS) -mno-red-zone -Werror=stack-usage=145 -c $< -o $@

../build/kernel/panic.o: panic.c $(INCLUDES)
	@mkdir -p $(@D)
	@echo "CC    $< => $(subst ../build/kernel/,,$@)"
	@x86_64-elf-gcc $(CFLAGS) -mno-red-zone -Werror=stack-usage=192 -c $< -o $@

../build/kernel/font.o:
	@mkdir -p $(@D)
	@echo "Building Font..."
	@$(LD) -r -b binary font.psf -o ../build/kernel/font.o

../test: ../test.c
	$(error Hey!  Fix the ELF tester binaries first!)

../test.c:
	$(error Hey!  Fix the ELF tester binaries first!)

../build/kernel/%.o: %.S
	@mkdir -p $(@D)
	@echo "AS    $< => $(subst ../build/kernel/,,$@)"
	@x86_64-elf-gcc -mno-red-zone -c $< -o $@

clean:
	@rm -f $(compile) ../bin/*.elf || true

softclean:
	@rm -f $(compile) || true
