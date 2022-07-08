cd kernel/kernel
x86_64-elf-gcc -Wall -fpic -ffreestanding -fno-stack-protector -mno-red-zone -I../include -I../../libc/include -c testabcd.c -o kernel.o
cd misc
x86_64-elf-ld -r -b binary -o fontpsf.o font.psf
cd ..
x86_64-elf-ld -nostdlib -L/opt/cross/lib/gcc/x86_64-elf/13.0.0 -lgcc -T ../arch/x86_64/linker.ld kernel.o misc/fontpsf.o -o kernel.elf
x86_64-elf-strip -s -K mmio -K fb -K bootboot -K environment -K initstack kernel.elf
x86_64-elf-readelf -hls kernel.elf > kernel.txt