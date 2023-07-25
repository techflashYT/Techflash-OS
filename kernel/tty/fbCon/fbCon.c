#include <kernel/tty.h>
#include <kernel/newFont.h>
#include <kernel/hardware/serial.h>
#include <kernel/environment.h>
#include <stdio.h>
#include <stdint.h>

static void FB_DrawChar(const char ch, const uint_fast16_t xPos, const uint_fast16_t yPos, const uint32_t color, const uint32_t bgColor);
static bool nextCharIsEsc = false;
#pragma GCC optimize "O0"
static uint_fast8_t charSpacing = 0;



// Specifications
static int MAX_BUFFER_SIZE = 255;
#define FLUSH_THRESHOLD (MAX_BUFFER_SIZE * 75 / 100)



static bufferEntry_t  bufferStatic[255];
static bufferEntry_t *buffer = bufferStatic;
int bufCount = 0;

void FB_WriteChar(const char ch, const uint_fast16_t x, const uint_fast16_t y) {
	bufferEntry_t entry;
	entry.ch = ch;
	entry.x = x;
	entry.y = y;
	entry.color = TTY_Color;
	entry.background = TTY_TextBackground;

	buffer[bufCount] = entry;
	bufCount++;

	// Check if buffer exceeds threshold
	if (bufCount >= FLUSH_THRESHOLD) {
		FB_Update();
	}
}

void FB_Update() {
	int i;
	for (i = 0; i < bufCount; i++) {
		bufferEntry_t entry = buffer[i];
		FB_DrawChar(entry.ch, entry.x, entry.y, entry.color, entry.background);
	}
	bufCount = 0; // Reset buffer
}

void FB_ReInit(bufferEntry_t* newBuffer, int newBufferSize) {
	// Assuming the new buffer has been allocated using malloc with specified new size
	// Copy from old buffer to new
	for (int i = 0; i < bufCount; i++) {
		newBuffer[i] = buffer[i];
	}
	// Make the new buffer as the active buffer
	buffer = newBuffer;
	// Resize the buffer size
	MAX_BUFFER_SIZE = newBufferSize;
}

char str[300];
static void FB_DrawChar(const char ch, const uint_fast16_t xPos, const uint_fast16_t yPos, const uint32_t color, const uint32_t bgColor) {
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
	uint32_t textColor = color;
	if (TTY_Bold) {
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
		for (uint_fast8_t i = 0; i != 4; i++) {
			uint8_t originalByte = ((uint8_t*)&color)[i];
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


	UNPACK_WIDTH_HEIGHT(font.header.widthHeight, width, height);


	uint_fast64_t fbLine = (TTY_CursorY * height);
	uint_fast64_t fbIndex = (TTY_CursorX * (width + charSpacing));
	uint_fast8_t i;
	for (i = 0; i != font.header.numGlyphs; i++) {
		if (font.glyphs[i].glyphNum == ch) {
			break;
		}
	}
	if (i == font.header.numGlyphs) {
		i = font.header.unkCharGlyph;
	}
	// sprintf(str, "i = 0x%04X\r\nnum: %04X\r\n", i, font.header.numGlyphs);
	// serial.writeString(SERIAL_PORT_COM1, str);
	// find the specified glyph
	uint8_t *bitfield = font.glyphs[i].glyphBitField;
	
	// iterate through each bit of the glyph
	for (uint_fast8_t j = 0; j < height; j++) {
		uint8_t row = bitfield[j];
		for (uint_fast8_t k = 0; k < width; k++){
			((uint32_t *)fb->address)[(fbIndex++) + (fbLine * fb->width)] = (row & (1 << (7 - k))) ? textColor : bgColor;
		}
		fbLine++;
		fbIndex = (TTY_CursorX * (width + charSpacing));
	}
	
	TTY_CursorX++;
	if (TTY_CursorX >= TTY_Width) {
		TTY_CursorX = 0;
		TTY_CursorY++;
	}
	if (TTY_CursorY >= TTY_Height) {
		TTY_Scroll(1);
	}

}