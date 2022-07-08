#!/bin/sh
if [ "$1" != "--no-pull" ]
then
	git pull
fi
if ! command -v i686-elf-gcc > /dev/null || ! command -v i686-elf-as > /dev/null || ! command -v i686-elf-ld > /dev/null || ! command -v x86_64-elf-gcc > /dev/null || ! command -v x86_64-elf-as > /dev/null || ! command -v x86_64-elf-ld > /dev/null 
then
	echo "Uh oh!  Looks like you never installed the i686-elf-gcc/x86_64-elf-gcc cross compiler!  Please follow the steps in the wiki to install it."
	exit 1
fi
if [ ! -f .config ]
then
	echo "Uh oh!  Looks like never ran ./configure!  Please run ./configure and then run this script again."
	exit 2
fi
./clean.sh
if ! ./build.sh
then
	sleep 0.05
	echo "Uh oh!  Looks like something failed to build!  Please check the above output to see what went wrong."
	exit 3
fi
echo "ISO"
./iso.sh
echo "Done!  Boot ISO is in bin/TFOS_ISO.iso"
