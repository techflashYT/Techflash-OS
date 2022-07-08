#!/bin/bash
set -e
shopt -s extglob
. ./util/config.sh
for PROJECT in $PROJECTS; do
	(cd "$PROJECT" && DESTDIR="$SYSROOT" $MAKE 2> >(../util/rederr.sh >&2))
done
