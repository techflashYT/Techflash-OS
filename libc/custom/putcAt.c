#include <kernel/tty.h>
#include <stdio.h>
int putcAt(char c, int x, int y, int color) {
	int ox = TTY_CursorX;
	int o2x = TTY_CursorAfterPromptX;
	int oy = TTY_CursorY;
	int oc = TTY_Color;
	TTY_CursorX = x;
	TTY_CursorY = y;
	TTY_Color = color;
	int ret = putchar(c);
	TTY_CursorX = ox;
	TTY_CursorAfterPromptX = o2x;
	TTY_CursorY = oy;
	TTY_Color = oc;
	return ret;
}