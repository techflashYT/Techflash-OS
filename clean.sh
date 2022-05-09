#!/bin/sh
set -e
. ./util/config.sh

for PROJECT in $PROJECTS; do
  (cd "$PROJECT" && $MAKE clean)
done

rm -rf sysroot
rm -rf isodir
rm -rf bin/TFOS_ISO.iso
