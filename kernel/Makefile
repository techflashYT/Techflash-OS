CPUFLAGS   = -march=core2 -mno-3dnow -mno-mmx -m64 -mabi=sysv -mno-red-zone
FEATURES   = -fbuiltin -fdiagnostics-color=always -fPIE -ffreestanding -fstack-protector-all -fno-lto -fno-omit-frame-pointer
#OPTIMIZE   = -Ofast # NORMAL
# OPTIMIZE   = -O2 # STABLE
OPTIMIZE   = -O0 # DEBUG
WARNINGS   = -Wall -Wextra -Wunused -Wunreachable-code -Wno-uninitialized -Wno-strict-aliasing -Werror
CFLAGS     = $(WARNINGS) -g -nostdlib -I../include -imacros misc/defines.h -include kernel/kernel.h -include kernel/log.h -std=gnu2x $(CPUFLAGS) $(OPTIMIZE) $(FEATURES)
LDFLAGS    = -fdiagnostics-color=always -nostdlib -T linkerScript.ld -static -L../lib -e _start
LIBS       = -lk
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
	@$(CC) $(CFLAGS) -Werror=stack-usage=350 -c $< -o $@

../build/kernel/font.o:
	@mkdir -p $(@D)
	@echo "Building Font..."
	@perl ../util/navcc.perl vgafont.f16 > font.c
	@echo "CC    font.c => $(subst ../build/kernel/,,$@)"
	@$(CC) $(CFLAGS) -c font.c -o $@

../test: ../test.c
	$(error Hey!  Fix the ELF tester binaries first!)

../test.c:
	$(error Hey!  Fix the ELF tester binaries first!)

../build/kernel/%.o: %.S
	@mkdir -p $(@D)
	@echo "AS    $< => $(subst ../build/kernel/,,$@)"
	@$(CC) -mno-red-zone -c $< -o $@

clean:
	@rm -f $(compile) ../bin/*.elf || true

softclean:
	@rm -f $(compile) || true
