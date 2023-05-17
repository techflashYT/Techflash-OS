#!/bin/bash
set -e
if [ ! -f sysroot/boot/install ]; then
	printf "ERROR: Kernel not found!  Please run ./build.sh first.\r\n"
	exit 1
fi
cd bootboot
if [ ! -f bootboot.bin ]; then
	wget https://gitlab.com/bztsrc/bootboot/-/raw/master/dist/bootboot.bin -q --progress=bar --show-progress
fi
if [ ! -f bootboot.efi ]; then
	wget https://gitlab.com/bztsrc/bootboot/-/raw/master/dist/bootboot.efi -q --progress=bar --show-progress
fi
cd ..
cp -r sysroot isodir
mkdir -p isodir/BOOTBOOT
cp bootboot/bootboot.bin isodir/BOOTBOOT/BOOTBOOT.BIN
cp bootboot/bootboot.efi isodir/BOOTBOOT/BOOTBOOT.EFI

# Make initrd
mkdir -p tmp/initrd/sys
cp -r sysroot/* tmp/initrd/
cp "test" tmp/initrd/
cp misc/converted/panic_screen.bin tmp/initrd/panic_screen.sys

prevDir=$(pwd)

pushd tmp/initrd > /dev/null 2>&1
tar --format=ustar -czf "$prevDir"/isodir/BOOTBOOT/INITRD -- *
unset prevDir
popd > /dev/null 2>&1

cat bootboot/config > isodir/BOOTBOOT/CONFIG
mkdir -p isodir/boot/grub
cd isodir/boot/grub
mkdir fonts themes locale
cd ../../..

cd grub
cat ISO_GRUB_CFG.cfg > ../isodir/boot/grub/grub.cfg
cp grub_bg.png ../isodir/boot/bg.png
cp font.pf2 ../isodir/boot/grub/fonts/unicode.pf2
cd ..
grub=NONE
if ( command -v grub-mkrescue > /dev/null ); then
	grub=grub
fi
if ( command -v grub2-mkrescue > /dev/null ); then
	grub=grub2
fi
mkdir -p isodir/EFI/TechflashOS
${grub}-mkstandalone -O x86_64-efi \
	--modules="efi_gop efi_uga video_bochs video_cirrus gfxterm gettext png" \
	--themes="" \
	--disable-shim-lock \
	-o "isodir/EFI/TechflashOS/BOOTx64.EFI" "isodir/boot/grub/grub.cfg"
	
${grub}-mkstandalone -O i386-efi \
	--modules="efi_gop efi_uga video_bochs video_cirrus gfxterm gettext png" \
	--themes="" \
	--disable-shim-lock \
	-o "isodir/EFI/TechflashOS/BOOTIA32.EFI" "isodir/boot/grub/grub.cfg"


${grub}-mkrescue -o bin/TFOS_ISO.iso isodir

