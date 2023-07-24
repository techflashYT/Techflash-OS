#include <stdint.h>
#include <kernel/graphics.h>
colors_t colors;
void colorsInit() {
	colors.vga.black  = 0xFF000000;
	colors.vga.blue   = 0xFF0000AA;
	colors.vga.green  = 0xFF00AA00;
	colors.vga.cyan   = 0xFF00AAAA;
	colors.vga.red    = 0xFFAA0000;
	colors.vga.purple = 0xFFAA00AA;
	colors.vga.brown  = 0xFFAA5500;
	colors.vga.lgray  = 0xFFAAAAAA;
	colors.vga.dgray  = 0xFF555555;
	colors.vga.lblue  = 0xFF5555FF;
	colors.vga.lgreen = 0xFF55FF55;
	colors.vga.lcyan  = 0xFF55FFFF;
	colors.vga.lred   = 0xFFFF5555;
	colors.vga.pink   = 0xFFFF55FF;
	colors.vga.yellow = 0xFFFFFF00;
	colors.vga.white  = 0xFFFFFFFF;

	colors.error = errorColor;
	colors.warn  = warningColor;
}