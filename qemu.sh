#!/bin/sh
set -e
# check if bin/TFOS_ISO.iso exists
if [ ! -f bin/TFOS_ISO.iso ]; then
	. ./iso.sh
fi

qemu-system-"$(./util/target-triplet-to-arch.sh "$HOST")" -cdrom bin/TFOS_ISO.iso
