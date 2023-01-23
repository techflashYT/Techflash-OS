#!/bin/sh
if [ "$(id -u)" -ne 0 ]; then
	printf "\033[1;33mPlease enter your sudo password to be able to run QEMU with KVM!\033[0m\r\n"
	SUDO=sudo
fi
$SUDO qemu-system-x86_64 -m 512M,slots=2,maxmem=1G -cpu host --enable-kvm -cdrom bin/TFOS_ISO.iso -display gtk -s "$@"
