#include <stdint.h>
#include <kernel/graphics.h>
vga_t vga;
void VGAInit() {
	vga.colors.black  = 0x000000;
	vga.colors.blue   = 0x0000AA;
	vga.colors.green  = 0x00AA00;
	vga.colors.cyan   = 0x00AAAA;
	vga.colors.red    = 0xAA0000;
	vga.colors.purple = 0xAA00AA;
	vga.colors.brown  = 0xAA5500;
	vga.colors.lgray  = 0xAAAAAA;
	vga.colors.dgray  = 0x555555;
	vga.colors.lblue  = 0x5555FF;
	vga.colors.lgreen = 0x55FF55;
	vga.colors.lcyan  = 0x55FFFF;
	vga.colors.lred   = 0xFF5555;
	vga.colors.pink   = 0xFF55FF;
	vga.colors.yellow = 0xFFFF00;
	vga.colors.white  = 0xFFFFFF;
}

