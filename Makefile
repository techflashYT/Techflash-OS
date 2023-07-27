HOSTCC=$(shell command -v gcc 2> /dev/null)
ifeq ($(HOSTCC),)
HOSTCC=$(shell command -v clang 2> /dev/null)
endif

ifeq ($(HOSTCC),)
$(error No C compiler found. Please install gcc or clang)
endif

$(info Using $(HOSTCC) as the C compiler)


.PHONY: config clean all iso usb

all: iso

iso: bin/TFOS_ISO.iso

bin/TFOS_ISO.iso: bin/tfos_kernel.elf limine/limine-bios.sys limine/limine-bios-cd.bin limine/limine-uefi-cd.bin limine/limine-cmd



limine/%.%:
	@$(info Downloading $(notdir $@) from GitHub...)
	@wget https://github.com/limine-bootloader/limine/raw/v5.x-branch-binary/$(notdir $@) -O $@

limine/limine-cmd: limine/limine.c limine/limine-bios-hdd.h
	@$(info make cmd)


bin/tfos_kernel.elf:

#config: bin/configure
#	@bin/configure

# bin/configure: util/configure.c
#	@$(info CC    $< => $@)
#	@mkdir -p $(@D)
#	@$(HOSTCC) $(HOSTCLFAGS) $< -o $@


clean:
	rm -rf build
	rm -rf bin
	rm -rf lib
	rm -rf sysroot
	rm -rf isodir