#include <stdlib.h>
#include <string.h>
#include <kernel/font.h>
#include <kernel/environment.h>
#include <kernel/tty/tty.h>
#include <kernel/panic.h>
#include <kernel/misc.h>
#include <kernel/hardware/IO.h>
#include <kernel/hardware/PIT.h>
#include <kernel/hardware/serial.h>
extern bool timerReady;
int putchar(const int ch) {
	DUMPREGS;
	
	if (!(ch == '\r' || ch == '\n' || ch == '\t' || (ch >= 0x20 && ch <= 0x7E))) {
		// attempted to print an invalid character, die
		char *str = "Attempt to print an invalid character: 0xAAAAAAAAAAAAAAAA";
		uint8_t off = 41;
		char *buf = malloc(18);
		memset(buf, '\0', 18);
		utoa((uint8_t)ch, buf, 16);
		memcpy(str + off, buf, 16);
		panic(str, regs);
	}
	if (ch == '\r') { // CR ('\r')
		kernTTY.cursorX = 0;
		return '\r';
	}
	if (ch == '\n') { // CR ('\r')
		kernTTY.cursorY++;
		return '\n';
	}
	if (ch == '\t') { // Tab ('\t')
		kernTTY.cursorX += 4;
		return '\t';
	}
	if (kernTTY.cursorX >= kernTTY.width) {
		kernTTY.cursorX = 0;
		kernTTY.cursorY++;
	}
	if (kernTTY.cursorY >= kernTTY.height) {
		kernTTY.scroll("1");
	}
	fbConDrawChar(ch, kernTTY.cursorX, kernTTY.cursorY);
	return ch;
}