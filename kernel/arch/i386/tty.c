#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/tty.h>
#include "vga.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t* const VGA_MEMORY = (uint16_t*) 0xB8000;

static size_t terminalRow;
static size_t terminalCol;
static uint8_t terminalColor;
static uint16_t* terminalBuf;

void terminalInit(void) {
	terminalRow = 0;
	terminalCol = 0;
	terminalColor = vgaEntryColor(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminalBuf = VGA_MEMORY;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminalBuf[index] = vgaEntry(' ', terminalColor);
		}
	}
}

void terminalSetColor(uint8_t color) {
	terminalColor = color;
}

void terminalPutCharAt(unsigned char c, uint8_t color, size_t x, size_t y) {
	if (c == '\r' || c == '\n') {
		return;
	}
	const size_t index = y * VGA_WIDTH + x;
	terminalBuf[index] = vgaEntry(c, color);
}
void terminalMove(size_t x, size_t y) {
	terminalRow = y;
	terminalCol = x;
}
size_t terminalGetX() {
	return terminalCol;
}
size_t terminalGetY() {
	return terminalRow;
}
bool checkIfNextCharIsNewline = false;
void terminalPutchar(char c) {
	unsigned char uc = (unsigned char)c;
	if (c == '\r') {
		checkIfNextCharIsNewline = true;                  // Begin check for newline next time this function is called
		return;
	}
	else if (c == '\n' && checkIfNextCharIsNewline) {     // If the current character to print is a newline and the previous character was a carriage return    (CRLF)  (Windows)
		terminalRow++;
		terminalCol = 0;
		checkIfNextCharIsNewline = false;
		return;
	}
	else if (c != '\n' && checkIfNextCharIsNewline) {    // If the current character to print is not a newline and the previous character was a carriage return (CR)    (Mac)
		terminalRow++;
		terminalCol = 0;
		checkIfNextCharIsNewline = false;
		return;
	}
	else if (c == '\n') {                                // If the current character to print is a newline and the previous character was not a carriage return (LF)    (Unix)
		terminalRow++;
		terminalCol = 0;
		return;
	}
	terminalPutCharAt(uc, terminalColor, terminalCol, terminalRow);
	if (++terminalCol == VGA_WIDTH) {
		terminalCol = 0;
		if (++terminalRow == VGA_HEIGHT) {
			terminalRow = 0;
		}
	}
}

void terminalWrite(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++) {
		terminalPutchar(data[i]);
	}
}

void terminalWritestring(const char* data) {
	terminalWrite(data, strlen(data));
}
