#include <stdlib.h>
#include <string.h>
#include <kernel/font.h>
#include <kernel/environment.h>
#include <kernel/tty.h>
#include <kernel/panic.h>
#include <kernel/misc.h>
#include <kernel/hardware/IO.h>
#include <kernel/hardware/PIT.h>
#include <kernel/hardware/serial.h>
extern bool timerReady;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
#pragma GCC diagnostic ignored "-Wsign-compare"
int putchar(const int ch) {
	DUMPREGS;
	
	if (!(ch == '\r' || ch == '\n' || ch == '\t' || (ch >= 0x20 && ch <= 0x7E))) {
		// attempted to print an invalid character, die
		char *str = "Attempt to print an invalid character: 0xAAAAAAAAAAAAAAAA";
		uint8_t off = 41;
		char *buf = malloc(18);
		memset(buf, '\0', 18);
		utoa((uint8_t)ch, buf, 16);
		memcpy(str + off, buf, 16);
		panic(str, regs);
	}
	if (ch == '\r') { // CR ('\r')
		kernTTY.cursorX = 0;
		return '\r';
	}
	if (ch == '\n') { // CR ('\r')
		kernTTY.cursorY++;
		return '\n';
	}
	if (ch == '\t') { // Tab ('\t')
		kernTTY.cursorX += 4;
		return '\t';
	}
	if (kernTTY.cursorX >= kernTTY.width) {
		kernTTY.cursorX = 0;
		kernTTY.cursorY++;
	}
	if (kernTTY.cursorY >= kernTTY.height) {
		kernTTY.scroll("1");
	}
	
	int bpl = (font->width + 7) / 8;
	int offs = 0;
	unsigned char *glyph = (unsigned char*)&_binary_font_psf_start + font->headerSize + (ch > 0 && ch < font->numOfGlyphs ? ch : 0) *font->bytesPerGlyph;
	int kx = kernTTY.cursorX;
	int offsY = (font->height * (((kernTTY.cursorY * bootboot.fb_width) * 4) == 0 ? 0 : (((kernTTY.cursorY * bootboot.fb_width) * 4))));
	// Moved these to the top of putchar in order to avoid issues of blinking interrupting the current character.
	kernTTY.cursorX++;
	kernTTY.cursorAfterPromptX++;
	
	offs = ((kx * (font->width + 1) * 4) + offsY);
	uint32_t y;
	for (y = 0; y < font->height; y++) {
		int line = 0;
		int mask = 0;
		line = offs;
		mask = 1 << (font->width - 1);
		uint32_t x;
		for (x = 0; x < font->width; x++) {
			*((uint32_t*)((uint64_t)&fb + line)) = (((int) *glyph) & (mask)) ? kernTTY.color : kernTTY.textBackground;
			mask >>= 1;
			line += 4;
		}
		*((uint32_t*)((uint64_t) &fb + line)) = kernTTY.textBackground;
		glyph += bpl;
		offs += bootboot.fb_scanline;
	}
	return ch;
}
#pragma GCC diagnostic pop