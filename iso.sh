#!/bin/bash
set -e
if [ ! -f sysroot/boot/tfos.elf ]; then
	printf "ERROR: Kernel not found!  Please run ./build.sh first.\r\n"
	exit 1
fi
if [ ! -f bootboot.bin ]; then
	wget https://gitlab.com/bztsrc/bootboot/-/raw/master/dist/bootboot.bin -q --progress=bar --show-progress
fi
cp -r sysroot isodir
mkdir isodir/BOOTBOOT
mv isodir/boot/tfos.elf isodir/boot/install
cp bootboot.bin isodir/BOOTBOOT/BOOTBOOT.BIN

# Make initrd
mkdir -p /tmp/initrd/sys
cp isodir/boot/install /tmp/initrd/sys/core
find /tmp/initrd | cpio -H crc -o | gzip > isodir/BOOTBOOT/INITRD
cat bootbootconfig > isodir/BOOTBOOT/CONFIG
mkdir -p isodir/boot/grub
cd isodir/boot/grub
mkdir fonts themes locale
cd ../../..
cat ISO_GRUB_CFG.cfg > isodir/boot/grub/grub.cfg
cp grub_bg.png isodir/boot/bg.png
cp font.pf2 isodir/boot/grub/fonts/unicode.pf2
grub-mkrescue -o bin/TFOS_ISO.iso isodir
# cp isodir/tfos.img bin/TFOS_ISO.iso




# if ! [ -f util/mkbootimg/mkbootimg ]; then
# 	echo "Grabbing latest mkbootimg & bootboot.bin"
# 	wget https://gitlab.com/bztsrc/bootboot/-/raw/master/dist/bootboot.bin?inline=false -q --progress=bar --show-progress -O bootboot.bin
# 	mv bootboot.bin isodir/boot/bootboot.bin
# 	cd util
# 	wget https://gitlab.com/bztsrc/bootboot/-/archive/master/bootboot-master.tar.bz2?path=mkbootimg -q --progress=bar --show-progress -O mkbootimg.tar.bz2
# 	tar -xf mkbootimg.tar.bz2 --warning=no-timestamp
# 	rm mkbootimg.tar.bz2
# 	mv bootboot-master-mkbootimg/mkbootimg ./mkbootimg
# 	rm -r bootboot-master-mkbootimg
# 	cd mkbootimg
# 	patch -u Makefile -i ../mkbootimg.patch -s
# 	cd ..
# 	if ! command -v fasm > /dev/null; then
# 		wget https://flatassembler.net/fasm-1.73.30.tgz -q --progress=bar --show-progress -O fasm.tgz
# 		tar -xf fasm.tgz
# 		rm fasm.tgz
# 		cd fasm
# 		echo "(If a \"[sudo]\" prompt doesn't pop up, please ignore this) Please enter your password so that we can install fasm, an assembler that is required by the bootloader of this project."
# 		sudo install -m 755 fasm /usr/bin/fasm
# 		cd ..
# 		rm -r fasm
# 	fi
# 	cd mkbootimg
# 	make &> /dev/null
# 	cd ../..
# fi
# cp bootbootconfig isodir
# cp bootboot.json isodir
# cd isodir
# ../util/mkbootimg/mkbootimg ./bootboot.json ./tfos.img
# if [ $? != 0 ]
# then
# 	exit 1
# fi
# cd ..

