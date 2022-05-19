#!/bin/sh
if [ "$1" != "--no-pull" ]
then
	git pull
fi
clear
./clean.sh
clear
./build.sh
./iso.sh
