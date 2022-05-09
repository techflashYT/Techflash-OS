#!/bin/sh
set -e
. ./iso.sh

qemu-system-"$(./util/target-triplet-to-arch.sh "$HOST")" -cdrom bin/TFOS_ISO.iso
