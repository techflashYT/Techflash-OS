#!/bin/sh
set -e
. ./build.sh


#cp sysroot/boot/tfos.elf isodir/boot/install
cp -r sysroot isodir
mv isodir/boot/tfos.elf isodir/boot/install
mkdir -p isodir/boot/grub
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "Install Techflash OS" {
	multiboot /boot/install
}
EOF
grub-mkrescue -o bin/TFOS_ISO.iso isodir
rm -r isodir
