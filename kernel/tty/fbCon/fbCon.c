#include <kernel/tty.h>
#include <kernel/newFont.h>
#include <kernel/hardware/serial.h>
#include <kernel/environment.h>
#include <stdio.h>
static bool nextCharIsEsc = false;
#pragma GCC optimize "O0"
static uint_fast8_t charSpacing = 8;


// Helper function to set a pixel at the given position with the specified color
void FB_Set(uint32_t* framebuffer, uint_fast16_t x, uint_fast16_t y, uint32_t color) {
	framebuffer[y * fb->pitch / 4 + x] = color;
}
void FB_DrawChar(const char ch, const uint_fast16_t xPos, const uint_fast16_t yPos) {
	(void)xPos;
	(void)yPos;
	if (nextCharIsEsc) {
		if (FB_HandleEsc(ch)) {
			nextCharIsEsc = false;
		}
		return;
	}
	if (ch == 0x1B) {
		nextCharIsEsc = true;
		return;
	}
	uint32_t textColor = TTY_Color;
	if (TTY_Bold) {
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
		for (uint_fast8_t i = 0; i < 4; i++) {
			uint8_t originalByte = ((uint8_t*)&TTY_Color)[i];
			((uint8_t *)&textColor)[i] += 0x55;

			// Check for overflow
			if (((uint8_t *)&textColor)[i] < originalByte) {
				((uint8_t *)&textColor)[i] = 0xFF;
			}
		}

		#pragma GCC diagnostic pop
	}
	uint_fast8_t width;
	uint_fast8_t height;


	(void)width;
	(void)height;
	UNPACK_WIDTH_HEIGHT(font.header.widthHeight, width, height);

	for (uint_fast8_t i = 0; i != font.header.numGlyphs; i++) {
		if (font.glyphs[i].glyphNum == ch) {
			char str[64];
			sprintf(str, "found matching glyph at index %d, glyph: 0x%02X\r\n", i, font.glyphs[i].glyphNum);
			serial.writeString(SERIAL_PORT_COM1, str);
			break;
		}
	}


	TTY_CursorX++;
	if (((TTY_CursorX) * (width + charSpacing)) > TTY_Width) {
		TTY_CursorX = 0;
		TTY_CursorY++;
		if (((TTY_CursorY) * (height + charSpacing)) > TTY_Height) {
			TTY_Scroll("1");
		}
	}

}