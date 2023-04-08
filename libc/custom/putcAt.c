#include <kernel/tty.h>
#include <stdio.h>
int putcAt(char c, int_fast32_t x, int_fast32_t y, int color) {
	uint_fast32_t ox       = TTY_CursorX;
	uint_fast32_t o2x      = TTY_CursorAfterPromptX;
	uint_fast32_t oy       = TTY_CursorY;
	uint32_t oc            = TTY_Color;
	TTY_CursorX            = x;
	TTY_CursorY            = y;
	TTY_Color              = color;
	int ret                = putchar(c);
	TTY_CursorX            = ox;
	TTY_CursorAfterPromptX = o2x;
	TTY_CursorY            = oy;
	TTY_Color              = oc;
	return ret;
}