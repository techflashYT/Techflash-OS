// #include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <kernel/tty.h>
#include <kernel/newFont.h>
#include <kernel/environment.h>
#include <kernel/graphics.h>
#include <kernel/hardware/serial.h>
// static bool     TTY_NextBlinkShouldBeOn;
uint_fast32_t TTY_Index;
uint_fast32_t TTY_Height;
uint_fast32_t TTY_Width;
uint_fast32_t TTY_TextBackground;
uint_fast32_t TTY_Color;
uint_fast16_t TTY_CursorY;
uint_fast16_t TTY_CursorX;	
bool          TTY_BlinkingCursor;
bool          TTY_Ready;
volatile bool TTY_Bold;

void TTY_Init() {
	TTY_Ready     = false;
	TTY_CursorX   = 0;
	TTY_CursorY   = 0;
	TTY_Color     = colors.vga.lgray; // VGA light gray.
	// TTY_Width     = ((fb->width / font->width) * 0.885);
	// TTY_Height    = ((fb->height / font->height) * 0.99);
	// TTY_Width     = 80;
	// TTY_Height    = 25;
	uint_fast8_t width;
	uint_fast8_t height;
	if (memcmp(font.header.ident, "TFF", 3) != 0) {
		serial.writeString(SERIAL_PORT_COM1, "TFF header bad\r\n");
		while(true) {asm("cli;hlt");}
	}
	UNPACK_WIDTH_HEIGHT(font.header.widthHeight, width, height);
	serial.writeString(SERIAL_PORT_COM1, "TFF header correct, init font\r\n");
	TTY_Width     = (fb->width  / width );
	TTY_Height    = (fb->height / height);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
#pragma GCC diagnostic ignored "-Wstringop-overflow"
void TTY_SetBackground(const uint32_t color) {
	TTY_TextBackground = color;
	for (uint32_t y = 0; y < fb->height; y++) {
		for (uint32_t x = 0; x < fb->width; x++) {
			*((uint32_t *)((uint64_t)fb->address * y + (x * 4))) = color;
		}
	}
}

void TTY_Clear() {
	// Set cursor to top left corner.
	TTY_CursorX = 0;
	TTY_CursorY = 0;
	// Fill the screen with the background color.
	for (uint32_t y = 0; y < fb->height; y++) {
		for (uint32_t x = 0; x < fb->width; x++) {
			*((uint32_t*)((uint64_t)fb->address * y + x * 4)) = TTY_TextBackground;
		}
	}
}
#pragma GCC diagnostic pop