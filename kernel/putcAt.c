#include <kernel/tty.h>
#include <stdio.h>
int putcAt(char c, int x, int y, int color) {
	int ox = kernTTY.cursorX;
	int oy = kernTTY.cursorY;
	int oc = kernTTY.color;
	kernTTY.cursorX = x;
	kernTTY.cursorY = y;
	kernTTY.color = color;
	int ret = putchar(c);
	kernTTY.cursorX = ox;
	kernTTY.cursorY = oy;
	kernTTY.color = oc;
	return ret;
}