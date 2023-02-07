#include <kernel/tty/tty.h>
#include <stdio.h>
int putcAt(char c, int x, int y, int color) {
	int ox = kernTTY.cursorX;
	int o2x = kernTTY.cursorAfterPromptX;
	int oy = kernTTY.cursorY;
	int oc = kernTTY.color;
	kernTTY.cursorX = x;
	kernTTY.cursorY = y;
	kernTTY.color = color;
	int ret = putchar(c);
	kernTTY.cursorX = ox;
	kernTTY.cursorAfterPromptX = o2x;
	kernTTY.cursorY = oy;
	kernTTY.color = oc;
	return ret;
}