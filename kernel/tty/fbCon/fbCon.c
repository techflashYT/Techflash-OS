#include <kernel/tty.h>
#include <kernel/newFont.h>
#include <kernel/hardware/serial.h>
#include <kernel/environment.h>
#include <stdio.h>
#include <stdint.h>
static bool nextCharIsEsc = false;
#pragma GCC optimize "O0"
static uint_fast8_t charSpacing = 8;


// Helper function to check if a specific bit in a glyph of the font is set
bool fontBitSet(uint8_t glyphNum, uint8_t x, uint8_t y) {
    if (glyphNum >= font.header.numGlyphs) {
        // Glyph number is out of range, return false
        return false;
    }

    // Get the glyph data from the font
    const uint8_t* glyphData = font.glyphs[glyphNum].glyphBitField;

    // Calculate the byte index and bit position within the byte
	uint_fast8_t width, height;
	UNPACK_WIDTH_HEIGHT(font.header.widthHeight, width, height);
	(void)height;

    uint8_t byteIndex = y * width;
    uint8_t bitPosition = x;

    // Check if the bit at the specified position is set (1) or not set (0)
    return (glyphData[byteIndex + (bitPosition / 8)] & (1 << (7 - (bitPosition % 8)))) != 0;
}
char str[300];
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


	uint_fast64_t fbLine = TTY_CursorY * height;
	uint_fast64_t fbIndex = (TTY_CursorX * (width + charSpacing));
	uint_fast8_t i;
	for (i = 0; i != font.header.numGlyphs; i++) {
		if (font.glyphs[i].glyphNum == ch) {
			break;
		}
	}

	sprintf(str, "found matching glyph at index %d, glyph: 0x%02X\r\n", i, font.glyphs[i].glyphNum);
	serial.writeString(SERIAL_PORT_COM1, str);
	// find the specified glyph
	uint8_t *bitfield = font.glyphs[i].glyphBitField;
	int arrayIndex = 0;
	
	// iterate through each bit of the glyph
	for (uint_fast8_t j = 0; j < height; j++) {
		uint8_t row = bitfield[j];
		for(uint_fast8_t k = 0; k < width; k++){
			str[arrayIndex++] = (row & (1 << (7 - k))) ? '#' : ' ';
			((uint32_t *)fb->address)[(fbIndex++) + (fbLine * fb->width)] = (row & (1 << (7 - k))) ? TTY_Color : TTY_TextBackground;
		}
		str[arrayIndex++] = '\r';
		str[arrayIndex++] = '\n';
		fbLine++;
		fbIndex = 0;
	}
	
	// Null-terminating the array to be safe when printing 
	str[arrayIndex] = '\0';

	serial.writeString(SERIAL_PORT_COM1, str);


	TTY_CursorX++;
	if (((TTY_CursorX) * (width + charSpacing)) > TTY_Width) {
		TTY_CursorX = 0;
		TTY_CursorY++;
		if (((TTY_CursorY) * (height + charSpacing)) > TTY_Height) {
			TTY_Scroll(1);
		}
	}

}