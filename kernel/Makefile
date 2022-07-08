# Original version Copyright 2017 - 2021 bzt (bztsrc@git.lab)
# as part of the 'bootboot' repository on GitLab
# Original File is availible at https://gitlab.com/bztsrc/bootboot/-/blob/master/mykernel/c/Makefile

CFLAGS     = -Wall -fpic -ffreestanding -fno-stack-protector -nostdinc -nostdlib -I../../dist/
LDFLAGS    = -nostdlib -lgcc -T linkerScript.ld
STRIPFLAGS = -s -K mmio -K fb -K bootboot -K environment -K initstack

compile=\
kernel.o

link=\
../build/kernel.o \
../build/font.o

EXPECTED_CONFIG_KERN_VERSION_MAJOR=0
EXPECTED_CONFIG_KERN_VERSION_MINOR=0
EXPECTED_CONFIG_KERN_VERSION_PATCH=6

include ../.config
RET := $(shell ../util/testVersion.sh $(CONFIG_KERN_VERSION_MAJOR) $(CONFIG_KERN_VERSION_MINOR) $(CONFIG_KERN_VERSION_PATCH) $(EXPECTED_CONFIG_KERN_VERSION_MAJOR) $(EXPECTED_CONFIG_KERN_VERSION_MINOR) $(EXPECTED_CONFIG_KERN_VERSION_PATCH))
ifneq ($(strip $(RET)),0)
$(error Kernel Version Mismatch!  Expected: $(EXPECTED_CONFIG_KERN_VERSION_MAJOR).$(EXPECTED_CONFIG_KERN_VERSION_MINOR).$(EXPECTED_CONFIG_KERN_VERSION_PATCH)  Got: $(CONFIG_KERN_VERSION_MAJOR).$(CONFIG_KERN_VERSION_MINOR).$(CONFIG_KERN_VERSION_PATCH) Error code: $(RET))
endif

.SUFFIXES: .o .c .S .S64

all: $(compile)
	@echo "Building Font..."
	@x86_64-elf-ld -r -b binary font.psf -o ../build/font.o
	@echo "CCLD  $(subst ../build/,,$(link)) => bin/tfos_kernel.elf"
	@x86_64-elf-gcc $(LDFLAGS) $(link) -o ../bin/tfos_kernel.elf
	@echo "Stripping Debug Symbols..."
	@x86_64-elf-strip $(STRIPFLAGS) ../bin/tfos_kernel.elf
	@echo "Making sysroot..."
	@make -f ../util/Makefile sysroot
# @x86_64-elf-readelf -hls mykernel.x86_64.elf >mykernel.x86_64.txt

.c.o:
	@echo "CC    $(subst .o,.c,$@) => $@"
	@x86_64-elf-gcc $(CFLAGS) -mno-red-zone -c $(subst .o,.c,$@) -o ../build/$@

clean:
	@rm -f *.o *.elf || true

softclean:
	@rm -f *.o || true
