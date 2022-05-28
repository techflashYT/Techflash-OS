#!/bin/sh
if [ "$1" != "--no-pull" ]
then
	git pull
fi
clear
./clean.sh
clear
./build.sh
echo "ISO"
./iso.sh
echo "Done!  Boot iso is in bin/TFOS_ISO.iso"
