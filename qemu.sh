#!/bin/bash
KVM="--enable-kvm "
CPU="${CPU:-default}"
CORES="2"
if [ "$1" = "--no-kvm" ]; then
	KVM=""
	set -- "" "${@:2:3}"
	if [ "$CPU" = "default" ]; then
		CPU="core2duo"
	fi
fi
if [ "$2" = "--no-kvm" ]; then
	KVM=""
	set -- "${@:1}" "" "${@:3}"
	CPU="core2duo"
fi

if [ "$(id -u)" -ne 0 ] && [ "$KVM" = "--enable-kvm " ]; then
	printf "\033[1;33mPlease enter your sudo password to be able to run QEMU with KVM!\033[0m\r\n"
	SUDO="sudo "
fi
if [ "$1" = "--cores" ]; then
	export CORES="$2"
	set -- "$3"
fi
if [ "$2" = "--cores" ]; then
	export CORES="$3"
	set -- "$1"
fi
if [ "$CPU" = "default" ]; then
	CPU="host"
fi
eval "$(echo "${SUDO}qemu-system-x86_64 -m 1024M -smp $CORES -cpu $CPU $KVM-cdrom bin/TFOS_ISO.iso -display gtk -s -d cpu_reset,int,pcall,unimp $* -serial stdio")" # fix obscure bug
