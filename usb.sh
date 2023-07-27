#!/bin/bash

# since we'll likely be running as root for writing to a device, lets make sure that we don't drop unusable files
# umask 00777
if ! [ -d isodir ]; then
	./iso.sh
fi

if [ "$1" = "" ]; then
	echo "please specify a device"
	exit 1
fi

dots() {
	sleep 0.25
	echo -n "$1"
	sleep 0.25
	echo -n "$1"
	sleep 0.25
	echo -n "$1"
	sleep 0.25
}

echo -e "\e[1;33m ===> WARNING:  This will erase all data on the device!!!!!!"
echo -n " ===> WARNING: Erasing ALL DATA in "
echo -ne "\e[32m5"
dots "."
echo -ne "4"
dots "."
echo -ne "\e[1;33m3"
dots "."
echo -ne "\e[0;33m2"
dots "."
echo -ne "\e[31m1"
dots "!"
echo -e "\e[0m"

sudo fdisk $1 << EOF
g
n


+4M
t
4
n



t

1
w
EOF
sudo limine/limine-cmd bios-install $1