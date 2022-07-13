#include <kernel/font.h>
#include <kernel/environment.h>
#include <kernel/tty.h>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
#pragma GCC diagnostic ignored "-Wsign-compare"
int putchar(const int ch) {
	if (ch == 0x0D) { // CR ('\r')
		kernTTY.cursorX = 0;
		return 0x0D;
	}
	if (ch == 0x0A) { // CR ('\r')
		kernTTY.cursorY++;
		return 0x0A;
	}
	
	psf2_t *font = (psf2_t*)&_binary_font_psf_start;
	int bpl = (font->width + 7) / 8;
	int offs;
	unsigned char *glyph = (unsigned char*)&_binary_font_psf_start + font->headersize + (ch > 0 && ch < font->numglyph ? ch : 0) *font->bytesperglyph;
	int kx = kernTTY.cursorX;
	offs = (kx * (font->width + 1) * 4);
	uint32_t y;
	for (y = 0; y < font->height; y++) {
		int line, mask;
		line = offs;
		mask = 1 << (font->width - 1);
		uint32_t x;
		for(x = 0; x < font->width; x++) {
			*((uint32_t*)((uint64_t)&fb + line)) = (((int)*glyph) & (mask)) ? kernTTY.color : 0;
			mask >>= 1;
			line += 4;
		}
		*((uint32_t*)((uint64_t)&fb+line)) = 0;
		glyph += bpl;
		offs += bootboot.fb_scanline;
	}
	kernTTY.cursorX++;
	return ch;
}
#pragma GCC diagnostic pop