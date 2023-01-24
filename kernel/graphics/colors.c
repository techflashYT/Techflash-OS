#include <stdint.h>
#include <kernel/graphics.h>
colors_t colors;
void colorsInit() {
	colors.vga.black  = 0x000000;
	colors.vga.blue   = 0x0000AA;
	colors.vga.green  = 0x00AA00;
	colors.vga.cyan   = 0x00AAAA;
	colors.vga.red    = 0xAA0000;
	colors.vga.purple = 0xAA00AA;
	colors.vga.brown  = 0xAA5500;
	colors.vga.lgray  = 0xAAAAAA;
	colors.vga.dgray  = 0x555555;
	colors.vga.lblue  = 0x5555FF;
	colors.vga.lgreen = 0x55FF55;
	colors.vga.lcyan  = 0x55FFFF;
	colors.vga.lred   = 0xFF5555;
	colors.vga.pink   = 0xFF55FF;
	colors.vga.yellow = 0xFFFF00;
	colors.vga.white  = 0xFFFFFF;

	colors.error = errorColor;
	colors.warn  = warningColor;
}