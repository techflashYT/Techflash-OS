#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <kernel/tty.h>
#include <kernel/font.h>
#include <kernel/environment.h>
#include <kernel/graphics.h>
// static bool     TTY_NextBlinkShouldBeOn;
uint_fast32_t TTY_Index;
uint_fast32_t TTY_Height;
uint_fast32_t TTY_Width;
uint_fast32_t TTY_TextBackground;
uint_fast32_t TTY_Color;
uint_fast16_t TTY_CursorAfterPromptX;
uint_fast16_t TTY_CursorY;
uint_fast16_t TTY_CursorX;	
bool          TTY_BlinkingCursor;
bool          TTY_Ready;

char         *TTY_PromptStr;
void TTY_Init() {
	TTY_Ready     = false;
	TTY_CursorX   = 0;
	TTY_CursorY   = 0;
	TTY_Color     = colors.vga.lgray; // VGA light gray.
	TTY_Width     = ((bootboot.fb_width / font->width) * 0.885);
	TTY_Height    = ((bootboot.fb_height / font->height) * 0.99);
	TTY_PromptStr = malloc(256);
	strcpy(TTY_PromptStr, "> ");
}
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
#pragma GCC diagnostic ignored "-Wstringop-overflow"
void TTY_SetBackground(const uint32_t color) {
	TTY_TextBackground = color;
	int s = bootboot.fb_scanline;
	for (uint32_t y = 0; y < bootboot.fb_height; y++) {
		for (uint32_t x = 0; x < bootboot.fb_width; x++) {
			*((uint32_t *)(&fb + s * y + x * 4)) = color;
		}
	}
}
void TTY_Clear() {
	int s = bootboot.fb_scanline;
	// Set cursor to top left corner.
	TTY_CursorX = 0;
	TTY_CursorY = 0;
	// Fill the screen with the background color.
	for (uint32_t y = 0; y < bootboot.fb_height; y++) {
		for (uint32_t x = 0; x < bootboot.fb_width; x++) {
			*((uint32_t*)(&fb + s * y + x * 4)) = TTY_TextBackground;
		}
	}
}
#pragma GCC diagnostic pop

void TTY_PrintPrompt() {
	if (TTY_CursorAfterPromptX != 0) {
		puts("\r\n");
	}
	puts(TTY_PromptStr);
	TTY_CursorAfterPromptX = 0;
}