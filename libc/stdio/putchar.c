#include <kernel/font.h>
#include <kernel/environment.h>
#include <kernel/tty.h>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
#pragma GCC diagnostic ignored "-Wsign-compare"
psf2_t *font;
int putchar(const int ch) {
	font = (psf2_t*)&_binary_font_psf_start;
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
	if (kernTTY.cursorX >= kernTTY.width - 15) {
		kernTTY.cursorX = 0;
		kernTTY.cursorY++;
	}
	
	int bpl = (font->width + 7) / 8;
	int offs = 0;
	unsigned char *glyph = (unsigned char*)&_binary_font_psf_start + font->headerSize + (ch > 0 && ch < font->numOfGlyphs ? ch : 0) *font->bytesPerGlyph;
	int kx = kernTTY.cursorX;
	int offsY = (font->height * (((kernTTY.cursorY * bootboot.fb_width) * 4) == 0 ? 0 : (((kernTTY.cursorY * bootboot.fb_width) * 4))));
	
	offs = ((kx * (font->width + 1) * 4) + offsY);
	uint32_t y;
	for (y = 0; y < font->height; y++) {
		int line = 0;
		int mask = 0;
		line = offs;
		mask = 1 << (font->width - 1);
		uint32_t x;
		for (x = 0; x < font->width; x++) {
			*((uint32_t*)((uint64_t)&fb + line)) = (((int) * glyph) & (mask)) ? kernTTY.color : kernTTY.textBackground;
			mask >>= 1;
			line += 4;
		}
		*((uint32_t*)((uint64_t) &fb + line)) = kernTTY.textBackground;
		glyph += bpl;
		offs += bootboot.fb_scanline;
	}
	kernTTY.cursorX++;
	kernTTY.cursorAfterPromptX++;
	return ch;
}
#pragma GCC diagnostic pop