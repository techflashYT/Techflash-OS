#include <stdlib.h>
#include <string.h>
int putchar(const int ch) {
	/*
	if (ch == '\r') { // CR ('\r')
		TTY_CursorX = 0;
		return '\r';
	}
	if (ch == '\n') { // CR ('\r')
		TTY_CursorY++;
		return '\n';
	}
	if (ch == '\t') { // Tab ('\t')
		TTY_CursorX += 4;
		return '\t';
	}
	if (ch == '\b') {
		TTY_CursorX--;
		putchar(' ');
		TTY_CursorX--;
		return '\b';
	}*/

	// FB_WriteChar(ch, TTY_CursorX, TTY_CursorY);
	return ch;
}
