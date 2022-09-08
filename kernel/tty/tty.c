#include <kernel/tty.h>
#include <kernel/font.h>
#include <kernel/environment.h>
#include <external/bootboot.h>
#include <stdbool.h>
#include <stdio.h>
char __kernTTY_buffer[32767] = "";
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
void updateScreen(const char* buffer) {
	// Clear screen
	
	__kernTTY_clear();
	// Draw text
	puts(buffer);
}