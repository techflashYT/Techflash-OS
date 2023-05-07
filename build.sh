#!/bin/bash
set -e
shopt -s extglob
. ./util/config.sh
for PROJECT in $PROJECTS; do
	(cd "$PROJECT" && DESTDIR="$SYSROOT" $MAKE && exit $?)
	if [ $? != 0 ];then
		exit 1
	fi
done


echo "Done!"