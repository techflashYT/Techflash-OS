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
# build symbols
echo "Generating kernel symbols file..."
if [ ! -f " bin/genSyms" ] || [ "bin/genSyms" -ot "symToFile.c" ]; then
	gcc -Ofast -march=native -mtune=native -fsanitize=address,undefined symToFile.c -o bin/genSyms
fi
bin/genSyms

echo "Done!"