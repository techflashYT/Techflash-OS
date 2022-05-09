#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/myos.kernel isodir/boot/myos.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "Install Techflash OS" {
	multiboot /boot/install
}
EOF
grub-mkrescue -o bin/TFOS_ISO.iso isodir
rm -r isodir
