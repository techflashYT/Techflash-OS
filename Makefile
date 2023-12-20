HOSTCC=$(shell command -v clang 2> /dev/null)

ifeq ($(HOSTCC),)
$(error [31mNo C compiler found. Please install clang.[0m)
endif

$(info Using $(HOSTCC) as the host C compiler)


GIT_HASH := $(shell git rev-parse --short HEAD)
MODIFIED := $(shell git status --porcelain)
AUTHOR   := $(shell git log -1 --pretty=format:'%an')

# Note: This does *not* make it so that when a contributor
# makes a commit, it becomes "unofficial"!  When I merge the PR
# on GitHub, it makes a commit as me after merging in the
# contributor's commit(s).
ifeq ($(AUTHOR), techflashYT)
   OFFICIAL := -official
else
   OFFICIAL := -unofficial
endif

ifeq ($(MODIFIED),)
   MOD := 
else
   MOD := -(!~MOD-nocommit~!)
endif

GIT_STR := $(GIT_HASH)$(OFFICIAL)$(MOD)
$(info git string is $(GIT_STR))


.PHONY: config clean all iso usb limineCoreFiles run libc kernel

all: iso

iso: bin/TFOS_ISO.iso


run:
	if [ "$$(uname -m)" = "x86_64" ]; then args=--enable-kvm; fi; \
		qemu-system-x86_64 -no-shutdown -no-reboot -m 256M -smp 2 -cdrom bin/TFOS_ISO.iso -display gtk -s -d int -serial stdio -cpu max $$args

limineCoreFiles: limine/limine-bios.sys limine/limine-bios-cd.bin limine/limine-uefi-cd.bin

bin/TFOS_ISO.iso: bin/tfos_kernel.elf isodir limineCoreFiles  limine/limine-cmd
	@$(info Creating ISO...)
	@xorriso -as mkisofs -b limine/limine-bios-cd.bin --no-emul-boot \
	 	--boot-load-size 4 --boot-info-table --efi-boot limine/limine-uefi-cd.bin \
		--efi-boot-part --efi-boot-image --protective-msdos-label isodir -o bin/TFOS_ISO.iso
	
	@$(info Adding Limine BIOS to image...)
	@limine/limine-cmd bios-install bin/TFOS_ISO.iso
	@$(info Done!)


isodir: bin/tfos_kernel.elf limineCoreFiles
	@mkdir -p isodir/limine
	@mkdir -p isodir/EFI/BOOT
	@mkdir -p isodir/boot
	@cp limine/limine-bios.sys limine/limine-bios-cd.bin limine/limine-uefi-cd.bin limine/limine.cfg isodir/limine/
	@cp bin/tfos_kernel.elf isodir/boot/install
	@for f in AA64 RISCV64 X64 IA32; do mcopy -ni limine/limine-uefi-cd.bin ::EFI/BOOT/BOOT$$f.EFI isodir/EFI/BOOT/; done


limine/%:
	@$(info Downloading $(notdir $@) from GitHub...)
	@wget https://github.com/limine-bootloader/limine/raw/v5.x-branch-binary/$(notdir $@) -O $@

limine/limine-cmd: limine/limine.c limine/limine-bios-hdd.h
	@$(info CC    limine.c ==> limine-cmd)
	@cd limine;\
		$(HOSTCC) limine.c -o limine-cmd;\
		cd ../

libc:
	@$(MAKE) -C libc HOSTCC=$(HOSTCC)

kernel: libc
	@$(MAKE) -C kernel HOSTCC=$(HOSTCC) GIT_STR='"$(GIT_STR)"'

bin/tfos_kernel.elf: libc kernel

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