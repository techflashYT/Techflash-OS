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

bin/TFOS_ISO.iso: bin/tfos_kernel.elf isodir limine/limine-bios.sys limine/limine-bios-cd.bin limine/limine-uefi-cd.bin limine/limine-cmd
	@$(info Creating ISO...)
	@xorriso -as mkisofs -b limine/limine-bios-cd.bin --no-emul-boot \
	 	--boot-load-size 4 --boot-info-table --efi-boot limine/limine-uefi-cd.bin \
		--efi-boot-part --efi-boot-image --protective-msdos-label isodir -o bin/TFOS_ISO.iso
	
	@$(info Adding Limine BIOS to image...)
	@limine/limine-cmd bios-install bin/TFOS_ISO.iso
	@$(info Done!)


isodir: bin/tfos_kernel.elf
	@mkdir -p isodir

limine/%.%:
	@$(info Downloading $(notdir $@) from GitHub...)
	@wget https://github.com/limine-bootloader/limine/raw/v5.x-branch-binary/$(notdir $@) -O $@

limine/limine-cmd: limine/limine.c limine/limine-bios-hdd.h
	@$(info CC    limine.c ==> limine-cmd)
	@cd limine;\
		$(HOSTCC) limine.c -o limine-cmd;\
		cd ../


bin/tfos_kernel.elf:
	@$(MAKE) -C kernel

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