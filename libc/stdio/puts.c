#include <stdint.h>
#include <kernel/font.h>
#include <kernel/bootboot.h>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Warray-bounds"
extern volatile unsigned char _binary_kernel_misc_font_psf_start;
void puts(char *s) {
	psf2_t *font = (psf2_t*)&_binary_kernel_misc_font_psf_start;
	int x, y, kx = 0, line, mask, offs;
	int bpl = (font->width + 7) / 8;
	while(*s) {
		unsigned char *glyph = (unsigned char*)&_binary_kernel_misc_font_psf_start + font->headerSize + (*s > 0 && *s < font->numberOfGlyphs ? *s : 0) * font->bytesPerGlyph;
		offs = (kx * (font->width + 1) * 4);
		for (y = 0; y < font->height; y++) {
			line = offs;
			mask = 1 << (font->width - 1);
			for(x = 0; x < font->width; x++) {
				*((uint32_t*) ((uint64_t) &fb + line)) = ((int) *glyph) & (mask) ? 0xFFFFFF : 0;
				mask >>= 1;
				line += 4;
			}
			*((uint32_t*) ((uint64_t) &fb + line)) = 0; glyph += bpl; offs += bootboot.fb_scanline;
		}
		s++;
		kx++;
	}
}
#pragma GCC diagnostic pop
