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

mkdir -p isodir/boot/grub/fonts

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
# ${grub}-mkstandalone -O x86_64-efi \
# 	--modules="efi_gop efi_uga video_bochs video_cirrus gfxterm gettext png" \
# 	--themes="" \
# 	--disable-shim-lock \
# 	-o "isodir/EFI/TechflashOS/BOOTx64.EFI" "isodir/boot/grub/grub.cfg"
	
# ${grub}-mkstandalone -O i386-efi \
# 	--modules="efi_gop efi_uga video_bochs video_cirrus gfxterm gettext png" \
# 	--themes="" \
# 	--disable-shim-lock \
# 	-o "isodir/EFI/TechflashOS/BOOTIA32.EFI" "isodir/boot/grub/grub.cfg"

${grub}-mkrescue -o bin/TFOS_ISO.iso isodir --themes=""

