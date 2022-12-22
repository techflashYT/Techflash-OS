#!/bin/bash
set -e
if [ ! -f sysroot/boot/tfos.elf ]; then
	printf "ERROR: Kernel not found!  Please run ./build.sh first.\r\n"
	exit 1
fi
if [ ! -f bootboot/bootboot.bin ]; then
	cd bootboot
	wget https://gitlab.com/bztsrc/bootboot/-/raw/master/dist/bootboot.bin -q --progress=bar --show-progress
	cd ..
fi
cp -r sysroot isodir
mkdir isodir/BOOTBOOT
mv isodir/boot/tfos.elf isodir/boot/install
cp bootboot/bootboot.bin isodir/BOOTBOOT/BOOTBOOT.BIN

# Make initrd
mkdir -p tmp/initrd/sys
cp -r sysroot/* tmp/initrd/
cp "test" tmp/initrd/
mv tmp/initrd/boot/tfos.elf tmp/initrd/sys/core
prevDir=$(pwd)
pushd tmp/initrd > /dev/null 2>&1
tar -czf $prevDir/isodir/BOOTBOOT/INITRD -- *
unset prevDir
popd > /dev/null 2>&1
rm -rf tmp/initrd
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
if ( command -v grub-mkrescue > /dev/null ); then
	grub-mkrescue /usr/lib/grub/i386-pc --stdio_sync off -o bin/TFOS_ISO.iso isodir
else
	if ( command -v grub2-mkrescue > /dev/null ); then
		grub2-mkrescue /usr/lib/grub/i386-pc --stdio_sync off -o bin/TFOS_ISO.iso isodir
	fi
fi
