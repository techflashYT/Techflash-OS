# Based on kernel/Makefile: For copyright notices, please view that file.
WARN     = -Wall -Wextra -Wstack-protector -Wformat=2 -Wformat-security
FEATURE  = -fdiagnostics-color=always -fno-pic -ffreestanding -fno-stack-protector # don't use the stack protector, it isn't implemented for userspace yet
OPTIMIZE   = -Ofast # NORMAL
# OPTIMIZE   = -O2 # STABLE
# OPTIMIZE   = -O0 -g # DEBUG
CFLAGS   = $(WARN) $(FEATURE) $(OPTIMIZE) -nostdlib -I../include -g -static -s -std=gnu2x -mcmodel=kernel -march=core2 -mno-3dnow -mno-mmx

override compile_S   := $(shell find -O3 . -type f -name '*.S')
override compile_c   := $(shell find -O3 -L . -type f -name '*.c')
compile=$(addprefix ../build/userspaceLibcWrappers/,$(subst ./,,$(subst .c,.o,$(subst .S,.o,$(compile_S) $(compile_c)))))

vpath %.c ./
vpath %.h ../include
outFileName=libc.a # any userspace applications for techflash os should link against this

include ../.config

.SUFFIXES: .o .c .S

all: ../lib/$(outFileName)

../lib/$(outFileName): $(compile)
	@mkdir -p ../lib
	@echo "Generating libc archive: $(outFileName)"
	@$(AR) cr ../lib/$(outFileName) $(compile)
	@echo "C library ready for use with userspace apps."

../build/userspaceLibcWrappers/%.o: %.c $(INCLUDES)
	@mkdir -p $(@D)
	@echo "CC    $< => $(subst ../build/userspaceLibcWrappers/,,$@)"
	@$(CC) $(CFLAGS) -mno-red-zone -c $< -o $@
../build/userspaceLibcWrappers/%.o: %.S
	@mkdir -p $(@D)
	@echo "AS    $< => $(subst ../build/userspaceLibcWrappers/,,$@)"
	@$(CC) $(CFLAGS) -mno-red-zone -c $< -o $@


clean:
	@rm -f $(compile) ../build/userspaceLibcWrappers/*.a || true

softclean:
	@rm -f $(compile) || true
