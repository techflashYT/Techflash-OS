#!/bin/bash
set -e
# Check if sysroot/boot/tfos.elf exists
if [ ! -f sysroot/boot/tfos.elf ]; then
	. ./build.sh
fi


#cp sysroot/boot/tfos.elf isodir/boot/install
cp -r sysroot isodir
mv isodir/boot/tfos.elf isodir/boot/install
mkdir -p isodir/boot/grub
mkdir -p isodir/boot/grub/{fonts,themes,locale}
cat > isodir/boot/grub/grub.cfg << EOF
$(cat ISO_GRUB_CFG.cfg)
EOF
cp grub_bg.png isodir/boot/bg.png
cp font.pf2 isodir/boot/grub/fonts/unicode.pf2
grub-mkrescue -o bin/TFOS_ISO.iso isodir &> /dev/null
rm -r isodir
