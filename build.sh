#!/bin/bash
set -e
. ./util/headers.sh

for PROJECT in $PROJECTS; do
	(cd "$PROJECT" && DESTDIR="$SYSROOT" $MAKE install 2> >(../util/rederr.sh >&2))
done

# Do a little filesystem cleanup
mkdir -p "$SYSROOT/usr/include/TechflashOS"
mv "$SYSROOT"/usr/include/* "$SYSROOT"/usr/include/TechflashOS/
