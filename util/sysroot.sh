#!/bin/sh
set -eu
if [ "$1" != "doIt" ]; then
	echo "This script is not for external usage!  It is meant to be called by the build script."
	exit 1
fi
cd ..
rm -rf sysroot
mkdir sysroot
cd sysroot
mkdir -p bin usr/include/TechflashOS usr/src/TechflashOS boot installContents
cp ../bin/tfos_kernel.elf boot/install
pwd
rsync -ah ../ usr/src/TechflashOS --exclude build --exclude .git --exclude sysroot --exclude isodir --exclude build --exclude tmp
