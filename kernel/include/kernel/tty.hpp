#ifndef _KERNEL_TTY_HPP
#define _KERNEL_TTY_HPP

#if defined VGA_WIDTH || defined VGA_HEIGHT
#undef VGA_WIDTH
#undef VGA_HEIGHT
#endif

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#include <stdint.h>
#include <stddef.h>
extern "C" {
	void terminalInit(void);
	void terminalScrollDown();
	void terminalSetColor(uint8_t color);
	void terminalPutEntryAt(uint8_t c, uint8_t color, size_t x, size_t y);
	void terminalPutchar(uint8_t c);
	void terminalClear();
	extern size_t terminalRow;
	extern size_t terminalColumn;
	extern uint8_t terminalColor;
	extern uint16_t* terminalBuffer;
	void terminalWrite(const char* data, size_t size);
	void terminalWriteString(const char* data);
}
void terminalWrite(const uint8_t *data, size_t size);
#endif
