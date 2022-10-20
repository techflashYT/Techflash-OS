#include <kernel/tty.h>
#include <kernel/font.h>
#include <kernel/environment.h>
#include <external/bootboot.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <kernel/graphics.h>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
#pragma GCC diagnostic ignored "-Wstringop-overflow"
void __kernTTY_init() {
	psf2_t *font = (psf2_t*)&_binary_font_psf_start;
	kernTTY.ready = false;
	kernTTY.cursorX = 0;
	kernTTY.cursorY = 0;
	kernTTY.color = 0xAAAAAA; // VGA light gray.
	kernTTY.width = (bootboot.fb_width / font->width);
	kernTTY.height = (bootboot.fb_height / font->height);
	kernTTY.promptStr = malloc(256);
	strcpy(kernTTY.promptStr, "> ");
}
void __kernTTY_setBackground(const uint32_t color) {
	kernTTY.textBackground = color;
	int s = bootboot.fb_scanline;
	for (uint32_t y = 0; y < bootboot.fb_height; y++) {
		for (uint32_t x = 0; x < bootboot.fb_width; x++) {
			*((uint32_t*)(&fb + s * y + x * 4)) = color;
		}
	}
}
void __kernTTY_clear() {
	int s = bootboot.fb_scanline;
	// Set cursor to top left corner.
	kernTTY.cursorX = 0;
	kernTTY.cursorY = 0;
	// Fill the screen with the background color.
	for (uint32_t y = 0; y < bootboot.fb_height; y++) {
		for (uint32_t x = 0; x < bootboot.fb_width; x++) {
			*((uint32_t*)(&fb + s * y + x * 4)) = kernTTY.textBackground;
		}
	}
}
#pragma GCC diagnostic pop

void __kernTTY_printPrompt() {
	puts(kernTTY.promptStr);
	kernTTY.cursorAfterPromptX = 0;
}
// FIXME: The cursor doesn't get erased properly
bool blink = false;
bool needsToEraseBlink = true;
uint16_t x = 0;
uint16_t y = 0;
extern uint32_t PITTick;
uint16_t shouldWait = 0;
extern char* lastNextKey;
void __kernTTY_blink() {
	if (shouldWait != 0) {
		needsToEraseBlink = true;
		x = kernTTY.cursorX;
		y = kernTTY.cursorY;
		putcAt('_', x, y, vga.colors.lgray);
		shouldWait--;
		if (lastNextKey[0] == '\b') {
			putcAt(' ', x + 1, y, vga.colors.lgray);
		}
		return;
	}
	
	if (kernTTY.blinkingCursor) {
		needsToEraseBlink = true;
		if (kernTTY.nextBlinkShouldBeOn) {
			shouldWait = 250;
			x = kernTTY.cursorX;
			y = kernTTY.cursorY;
			kernTTY.nextBlinkShouldBeOn = false;
			blink = true;
			putcAt('_', x, y, vga.colors.lgray);
			return;
		}
		
		if ((PITTick % 250) == 0) {
			if (!blink) {
				x = kernTTY.cursorX;
				y = kernTTY.cursorY;
				blink = true;
				putcAt('_', x, y, vga.colors.lgray);
			}
			else {
				blink = false;
				if (y != kernTTY.cursorY) {
					putcAt(' ', x, y, vga.colors.lgray);
				}
				if (y == kernTTY.cursorY) {
					if (x >= kernTTY.cursorX) {
						putcAt(' ', x, y, vga.colors.lgray);
					}
				}
			}
		}
	}
	else {
		if (needsToEraseBlink) {
			needsToEraseBlink = false;
			putcAt(' ', kernTTY.cursorX, kernTTY.cursorY, vga.colors.lgray);
		}
	}
}