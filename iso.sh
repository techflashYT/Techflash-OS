#!/bin/bash
set -e
if [ ! -f sysroot/boot/install ]; then
	printf "ERROR: Kernel not found!  Please run ./build.sh first.\r\n"
	exit 1
fi
cp -r sysroot isodir
mkdir -p isodir

# Make initrd
mkdir -p tmp/initrd/sys
cp -r sysroot/* tmp/initrd/
cp "test" tmp/initrd/
cp misc/converted/panic_screen.bin tmp/initrd/panic_screen.sys

prevDir=$(pwd)

pushd tmp/initrd
tar --format=ustar -cf "$prevDir"/isodir/boot/initrd -- *
unset prevDir
popd

if ! [ -f limine/limine.c ]; then
	wget https://raw.githubusercontent.com/limine-bootloader/limine/v5.x-branch-binary/limine.c -O limine/limine.c
fi
if ! [ -f limine/limine-bios-hdd.h ]; then
	wget https://raw.githubusercontent.com/limine-bootloader/limine/v5.x-branch-binary/limine-bios-hdd.h -O limine/limine-bios-hdd.h
fi
if ! [ -f limine/limine-cmd ]; then
	echo "CC    limine.c ==> limine-cmd"
	cd limine
	gcc limine.c -o limine-cmd
	cd ../
fi

if ! [ -f limine/limine-bios.sys ]; then
	wget https://github.com/limine-bootloader/limine/raw/v5.x-branch-binary/limine-bios.sys -O limine/limine-bios.sys
fi
if ! [ -f limine/limine-uefi-cd.bin ]; then
	wget https://github.com/limine-bootloader/limine/raw/v5.x-branch-binary/limine-uefi-cd.bin -O limine/limine-uefi-cd.bin
fi
if ! [ -f limine/limine-bios-cd.bin ]; then
	wget https://github.com/limine-bootloader/limine/raw/v5.x-branch-binary/limine-bios-cd.bin -O limine/limine-bios-cd.bin
fi

mkdir -p isodir/limine
cp limine/limine-{bios.sys,uefi-cd.bin,bios-cd.bin} limine/limine.cfg isodir/limine
xorriso -as mkisofs -b limine/limine-bios-cd.bin --no-emul-boot --boot-load-size 4 --boot-info-table --efi-boot limine/limine-uefi-cd.bin --efi-boot-part --efi-boot-image --protective-msdos-label isodir -o bin/TFOS_ISO.iso
limine/limine-cmd bios-install bin/TFOS_ISO.iso

# mkdir -p isodir/boot/grub/fonts

# cd grub
# cat ISO_GRUB_CFG.cfg > ../isodir/boot/grub/grub.cfg
# cp grub_bg.png ../isodir/boot/bg.png
# cp font.pf2 ../isodir/boot/grub/fonts/unicode.pf2
# cd ..
# grub=NONE
# if ( command -v grub-mkrescue > /dev/null ); then
# 	grub=grub
# fi
# if ( command -v grub2-mkrescue > /dev/null ); then
# 	grub=grub2
# fi
# # ${grub}-mkstandalone -O x86_64-efi \
# # 	--modules="efi_gop efi_uga video_bochs video_cirrus gfxterm gettext png" \
# # 	--themes="" \
# # 	--disable-shim-lock \
# # 	-o "isodir/EFI/TechflashOS/BOOTx64.EFI" "isodir/boot/grub/grub.cfg"
	
# # ${grub}-mkstandalone -O i386-efi \
# # 	--modules="efi_gop efi_uga video_bochs video_cirrus gfxterm gettext png" \
# # 	--themes="" \
# # 	--disable-shim-lock \
# # 	-o "isodir/EFI/TechflashOS/BOOTIA32.EFI" "isodir/boot/grub/grub.cfg"

# ${grub}-mkrescue -o bin/TFOS_ISO.iso isodir --themes=""

