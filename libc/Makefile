# Based on kernel/Makefile: For copyright notices, please view that file.
CPUFLAGS   = -march=core2 -mno-3dnow -mno-mmx -m64 -mabi=sysv -mno-red-zone
FEATURES   = -fbuiltin -fdiagnostics-color=always -fPIE -ffreestanding -fstack-protector-all -fno-lto -fno-omit-frame-pointer
#OPTIMIZE   = -Ofast # NORMAL
# OPTIMIZE   = -O2 # STABLE
OPTIMIZE   = -O0 # DEBUG
WARNINGS   = -Wall -Wextra -Wunused -Wunreachable-code -Wno-uninitialized -Wno-strict-aliasing -Werror
CFLAGS     = $(WARNINGS) -g -nostdlib -I../include -imacros misc/defines.h -include kernel/kernel.h -include kernel/log.h -std=gnu2x $(CPUFLAGS) $(OPTIMIZE) $(FEATURES)
INCLUDES = $(shell find -O3 ../include/ -type f)
override compile_S   := $(shell find -O3 . -type f -name '*.S')
override compile_c   := $(shell find -O3 . -type f -name '*.c')
compile=$(addprefix ../build/libc/,$(subst ./,,$(subst .c,.o,$(subst .S,.o,$(compile_S) $(compile_c)))))

vpath %.c ./
vpath %.S ./
vpath %.h ../include
outFileName=libk.a # libk.a for kernel library static library.  Not ready for hosted libc yet.

include ../.config

.SUFFIXES: .o .c .S

all: ../lib/$(outFileName)

../lib/$(outFileName): $(compile)
	@mkdir -p ../lib
	@echo "Generating libk archive: $(outFileName)"
	@$(AR) cr ../lib/$(outFileName) $(compile)
	@echo "C library ready for use with kernel."

../build/libc/%.o: %.c $(INCLUDES)
	@mkdir -p $(@D)
	@echo "CC    $< => $(subst ../build/libc/,,$@)"
	@$(CC) $(CFLAGS) -include kernel/kernel.h -include kernel/log.h -mno-red-zone -c $< -o $@

../build/libc/%.o: %.S $(INCLUDES)
	@mkdir -p $(@D)
	@echo "CC    $< => $(subst ../build/libc/,,$@)"
	@$(CC) $(CFLAGS) -mno-red-zone -c $< -o $@

clean:
	@rm -f $(compile) ../build/libc/*.a || true

softclean:
	@rm -f $(compile) || true
