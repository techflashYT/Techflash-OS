#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/tty.h>
#include <kernel/arch/i386/vga.h>

uint8_t vgaEntryColor(enum vgaColor fg, enum vgaColor bg) {
	return (uint8_t)(fg | bg << 4);
}

uint16_t vgaEntry(unsigned char uc, uint8_t color) {
	return (uint16_t) uc | (uint16_t) color << 8;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t vgaIndex(size_t x, size_t y) {
	return y * VGA_WIDTH + x;
}

size_t terminalRow;
size_t terminalColumn;
uint8_t terminalColor;
uint16_t* terminalBuffer;

void terminalInit(void) {
	terminalRow = 1;
	terminalColumn = 0;
	terminalColor = vgaEntryColor(VGA_COLOR_LIGHT_GRAY, VGA_COLOR_BLACK);
	terminalBuffer = (uint16_t*) 0xB8000;
	for (size_t y = 1; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminalBuffer[index] = vgaEntry('\0', terminalColor);
		}
	}
}



void terminalScrollDown() {
	size_t x, y;
	for(y = 0; y < VGA_HEIGHT - 1; y++) {
		for (x = 0; x < VGA_WIDTH; x++) {
			terminalBuffer[vgaIndex(x,y)] = terminalBuffer[vgaIndex(x,y+1)];
		}
	}
	for (x = 0; x < VGA_WIDTH; x++) {
		terminalBuffer[vgaIndex(x,y)] = vgaEntry(' ', terminalColor);
	}
}

void terminalSetColor(uint8_t color) {
	terminalColor = color;
}
 
void terminalPutEntryAt(char c, uint8_t color, size_t x, size_t y) {
	terminalBuffer[vgaIndex(x,y)] = vgaEntry((unsigned char)c, color);
}
void terminalPutchar(char c) {
	if (c == '\r') {
		terminalColumn = 0;
		return;
	}
	if (c == '\n') {
		terminalRow++;
		return;
	}
	
	terminalPutEntryAt(c, terminalColor, terminalColumn, terminalRow);
	
	if (++terminalColumn == VGA_WIDTH) {
		terminalColumn = 0;
		++terminalRow;
		if (terminalRow == VGA_HEIGHT) {
			--terminalRow;
			terminalScrollDown();
		}
	}
}

void terminalWrite(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++) {
		terminalPutchar(data[i]);
	}
}

void terminalWriteString(const char* data) {
	terminalWrite(data, strlen(data));
}

void terminalClear() {
	terminalRow = 0;
	terminalColumn = 0;
	for (uint8_t y = 0; y < VGA_HEIGHT; y++) {
		for (uint8_t x = 0; x < VGA_WIDTH; x++) {
			terminalBuffer[y * VGA_WIDTH + x] = 0;
		}
	}
}
