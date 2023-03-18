#!/bin/sh
KVM="--enable-kvm "
CPU="host"
if [ "$1" = "--no-kvm" ]; then
	KVM=""
	set -- ""
	CPU="core2duo"
fi
if [ "$(id -u)" -ne 0 ] && [ "$KVM" = "--enable-kvm " ]; then
	printf "\033[1;33mPlease enter your sudo password to be able to run QEMU with KVM!\033[0m\r\n"
	SUDO="sudo "
fi
eval "$(echo "${SUDO}qemu-system-x86_64 -m 512M,slots=2,maxmem=1G -smp 2 -cpu $CPU $KVM-cdrom bin/TFOS_ISO.iso -display gtk -s $*")" # fix obscure bug
