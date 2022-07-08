#!/bin/bash
set -e
if [ ! -f sysroot/boot/tfos.elf ]; then
	. ./build.sh
fi
cp -r sysroot isodir
mv isodir/boot/tfos.elf isodir/boot/install
mkdir -p isodir/boot/grub
mkdir -p isodir/boot/grub/{fonts,themes,locale}
cat > isodir/boot/grub/grub.cfg << EOF
$(cat ISO_GRUB_CFG.cfg)
EOF
cp grub_bg.png isodir/boot/bg.png
cp font.pf2 isodir/boot/grub/fonts/unicode.pf2
# grub-mkrescue -o bin/TFOS_ISO.iso isodir
cp bootboot.bin isodir/boot/bootboot.bin
cp bootbootconfig isodir
cp bootboot.json isodir
cd isodir
mkbootimg ./bootboot.json ./tfos.img
if [ $? != 0 ]
then
	exit 1
fi
cd ..


cp isodir/tfos.img bin/TFOS_ISO.iso