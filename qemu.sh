qemu-system-x86_64 -m 512M,slots=2,maxmem=1G -no-reboot -cpu core2duo -monitor stdio -no-shutdown -serial file:/proc/self/fd/1 -cdrom bin/TFOS_ISO.iso -s -S
