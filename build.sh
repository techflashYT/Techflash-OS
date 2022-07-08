#!/bin/bash
set -e
shopt -s extglob
. ./util/headers.sh

for PROJECT in $PROJECTS; do
	(cd "$PROJECT" && DESTDIR="$SYSROOT" $MAKE install 2> >(../util/rederr.sh >&2))
done

# Do a little filesystem cleanup
mkdir -p "$SYSROOT/usr/include/TechflashOS"
mv "$SYSROOT"/usr/include/!(TechflashOS) "$SYSROOT"/usr/include/TechflashOS/   # Move everything but the TechflashOS folder into TechflashOS
