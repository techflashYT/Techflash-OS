#include <stdint.h>
#include <kernel/graphics.h>
colors_t colors;
void colorsInit() {
	colors.vga.black  = 0x000000FF;
	colors.vga.blue   = 0x0000AAFF;
	colors.vga.green  = 0x00AA00FF;
	colors.vga.cyan   = 0x00AAAAFF;
	colors.vga.red    = 0xAA0000FF;
	colors.vga.purple = 0xAA00AAFF;
	colors.vga.brown  = 0xAA5500FF;
	colors.vga.lgray  = 0xAAAAAAFF;
	colors.vga.dgray  = 0x555555FF;
	colors.vga.lblue  = 0x5555FFFF;
	colors.vga.lgreen = 0x55FF55FF;
	colors.vga.lcyan  = 0x55FFFFFF;
	colors.vga.lred   = 0xFF5555FF;
	colors.vga.pink   = 0xFF55FFFF;
	colors.vga.yellow = 0xFFFF00FF;
	colors.vga.white  = 0xFFFFFFFF;

	colors.error = errorColor;
	colors.warn  = warningColor;
}