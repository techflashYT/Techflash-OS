#include <kernel/tty.h>
#include <kernel/font.h>
#include <kernel/environment.h>
bool nextCharIsEsc = false;
void FB_DrawChar(const char ch, const uint_fast16_t x, const uint_fast16_t y) {
	if (nextCharIsEsc) {
		FB_HandleEsc(ch);
	}
	if (ch == 0x1B) {
		nextCharIsEsc = true;
		return;
	}
	int_fast32_t bpl = (font->width + 7) / 8;
	int_fast32_t offs = 0;
	uint8_t *glyph = (uint8_t*)&_binary_font_psf_start + font->headerSize + (ch > 0 && (uint32_t)ch < font->numOfGlyphs ? ch : 0) *font->bytesPerGlyph;
	int_fast32_t kx = x;
	int_fast32_t offsY = (font->height * (((y * bootboot.fb_width) * 4) == 0 ? 0 : (((y * bootboot.fb_width) * 4))));
	TTY_CursorX++;
	TTY_CursorAfterPromptX++;
	
	offs = ((kx * (font->width + 1) * 4) + offsY);
	
	
	for (uint_fast32_t y2 = 0; y2 < font->height; y2++) {
		int_fast32_t line = 0;
		int_fast32_t mask = 0;
		line = offs;
		mask = 1 << (font->width - 1);
		for (uint_fast32_t x2 = 0; x2 < font->width; x2++) {
			uint32_t *ptr = ((uint32_t *)((uint64_t)&fb + line));
			*ptr = (((int) *glyph) & (mask)) ? TTY_Color : TTY_TextBackground;
			mask >>= 1;
			line += 4;
		}
		*((uint32_t *)((uint64_t) &fb + line)) = TTY_TextBackground;
		glyph += bpl;
		offs += bootboot.fb_scanline;
	}
	return;
}