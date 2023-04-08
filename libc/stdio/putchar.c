#include <stdlib.h>
#include <string.h>
#include <kernel/font.h>
#include <kernel/environment.h>
#include <kernel/tty.h>
#include <kernel/panic.h>
#include <kernel/hardware/IO.h>
#include <kernel/hardware/PIT.h>
#include <kernel/hardware/serial.h>
extern bool timerReady;
int putchar(const int ch) {
	/*DUMPREGS;
	
	if (!(ch == '\r' || ch == '\n' || ch == '\t' || (ch >= 0x20 && ch <= 0x7E))) {
		// attempted to print an invalid character, die
		char *str = "Attempt to print an invalid character: 0xAAAAAAAAAAAAAAAA";
		uint_fast8_t off = 41;
		char *buf = malloc(18);
		memset(buf, '\0', 18);
		utoa((uint_fast8_t)ch, buf, 16);
		memcpy(str + off, buf, 16);
		panic(str, regs);
	}
	*/
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
	if (TTY_CursorX >= TTY_Width) {
		TTY_CursorX = 0;
		TTY_CursorY++;
	}
	if (TTY_CursorY >= TTY_Height) {
		TTY_Scroll("1");
	}
	FB_DrawChar(ch, TTY_CursorX, TTY_CursorY);
	return ch;
}
