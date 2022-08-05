# Original version Copyright 2017 - 2021 bzt (bztsrc@git.lab)
# as part of the 'bootboot' repository on GitLab
# Original File is availible at https://gitlab.com/bztsrc/bootboot/-/blob/master/mykernel/c/Makefile
FEATURES   = -mavx -msse -msse2 -mno-sse3 -mno-sse4 -mno-sse4.1 -mno-sse4.2 -mno-sse4a -mno-xop -mno-fma4 -mno-cx16 -mno-avx -mno-avx2 -mno-avx512f -mno-3dnow -mno-mmx
CFLAGS     = -Wall -Wextra -Wno-uninitialized -fdiagnostics-color=always -fno-pic -ffreestanding -fno-stack-protector -nostdlib -g -Ofast -I../include -std=gnu2x -mcmodel=kernel $(FEATURES)
LDFLAGS    = -fdiagnostics-color=always -nostdlib -lgcc -T linkerScript.ld -static -L../lib
LIBS       = -lk
STRIPFLAGS = -s -K mmio -K fb -K bootboot -K environment -K initstack
SHELL=/bin/bash

include compLnk.mk


EXPECTED_CONFIG_KERN_VERSION_MAJOR=0
EXPECTED_CONFIG_KERN_VERSION_MINOR=1
EXPECTED_CONFIG_KERN_VERSION_PATCH=0

include ../.config
RET := $(shell ../util/testVersion.sh $(CONFIG_KERN_VERSION_MAJOR) $(CONFIG_KERN_VERSION_MINOR) $(CONFIG_KERN_VERSION_PATCH) $(EXPECTED_CONFIG_KERN_VERSION_MAJOR) $(EXPECTED_CONFIG_KERN_VERSION_MINOR) $(EXPECTED_CONFIG_KERN_VERSION_PATCH))
ifneq ($(strip $(RET)),0)
$(error Kernel Version Mismatch!  Expected: $(EXPECTED_CONFIG_KERN_VERSION_MAJOR).$(EXPECTED_CONFIG_KERN_VERSION_MINOR).$(EXPECTED_CONFIG_KERN_VERSION_PATCH)  Got: $(CONFIG_KERN_VERSION_MAJOR).$(CONFIG_KERN_VERSION_MINOR).$(CONFIG_KERN_VERSION_PATCH) Error code: $(RET))
endif

CFLAGS += -DCONFIG_KERN_VERSION_MAJOR=$(CONFIG_KERN_VERSION_MAJOR) -DCONFIG_KERN_VERSION_MINOR=$(CONFIG_KERN_VERSION_MINOR) -DCONFIG_KERN_VERSION_PATCH=$(CONFIG_KERN_VERSION_PATCH)
.SUFFIXES: .o .c .S

all: $(compile)
	@echo "Building Font..."
	$(shell x86_64-elf-ld -r -b binary font.psf -o ../build/kernel/font.o)
	@echo "CCLD  $(subst ../build/,,$(link)) => bin/tfos_kernel.elf"
	$(shell x86_64-elf-gcc $(LDFLAGS) $(link) $(LIBS) -o ../bin/tfos_kernel.elf  2> >(../util/rederr.sh >&2);exit $$?)
	@echo "Stripping Debug Symbols..."
#@x86_64-elf-strip $(STRIPFLAGS) ../bin/tfos_kernel.elf
	@echo "Making sysroot..."
	@make --no-print-directory -f ../util/Makefile sysroot

.c.o:
	@echo "CC    $(subst .o,.c,$@) => $@"
	@x86_64-elf-gcc $(CFLAGS) -mno-red-zone -c $(subst .o,.c,$@) -o ../build/kernel/$@
.S.o:
	@echo "AS    $(subst .o,.S,$@) => $@"
	@x86_64-elf-gcc $(CFLAGS) -mno-red-zone -c $(subst .o,.S,$@) -o ../build/kernel/$@
clean:
	@rm -f $(link) ../bin/*.elf || true

softclean:
	@rm -f $(link) || true