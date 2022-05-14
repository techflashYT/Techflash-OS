#!/bin/sh
set -e
. ./util/config.sh

for PROJECT in $PROJECTS; do
	if [ "$1" = "-s" ]
	then
		(cd "$PROJECT" && $MAKE softclean)
	else
		(cd "$PROJECT" && $MAKE clean)
	fi
done

rm -rf sysroot
rm -rf isodir
rm -rf bin/TFOS_ISO.iso
