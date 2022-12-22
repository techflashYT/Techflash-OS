#!/bin/sh
if [ "$1" != "--no-pull" ]; then
	git pull
fi
if ! ( command -v grub-mkrescue > /dev/null || command -v grub2-mkrescue > /dev/null ); then
	echo "Uh oh!  Looks like you don't have grub(2)-mkrescue and xorriso!  These are required to create the bootable ISO image."
	exit 1
fi
if ! ( command -v x86_64-elf-gcc > /dev/null || command -v x86_64-elf-as > /dev/null || command -v x86_64-elf-ld > /dev/null ); then
	echo "Uh oh!  Looks like you never installed the x86_64-elf-gcc cross compiler!  Please follow the steps in the wiki to install it."
	exit 1
fi
if [ ! -f .config ]; then
	echo "Uh oh!  Looks like never ran ./configure!  Please run ./configure and then run this script again."
	exit 2
fi
if ! ./build.sh; then
	sleep 0.05
	echo "Uh oh!  Looks like something failed to build!  Please check the above output to see what went wrong."
	exit 3
fi
echo "ISO"
./iso.sh

printf "Cleaning up temporary files...\r\n"
rm -rf sysroot isodir tmp
printf "Done!\r\n"
echo "Done!  Boot ISO is in bin/TFOS_ISO.iso"
