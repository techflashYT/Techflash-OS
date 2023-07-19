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
uint_fast16_t TTY_CursorY;
uint_fast16_t TTY_CursorX;	
bool          TTY_BlinkingCursor;
bool          TTY_Ready;
volatile bool TTY_Bold;
struct flanterm_context *TTY_Ctx; 

void TTY_Init() {
	TTY_Ctx = flanterm_fb_simple_init(fb->address, fb->width, fb->height, fb->pitch);
	char str[] = "hellorld?";
	flanterm_write(TTY_Ctx, str, sizeof(str));
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