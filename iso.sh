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
cat > isodir/boot/grub/grub.cfg << EOF
$(cat ISO_GRUB_CFG.cfg)
EOF
grub-mkrescue -o bin/TFOS_ISO.iso isodir &> /dev/null
rm -r isodir
