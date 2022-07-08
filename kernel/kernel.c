/*
	Original version Copyright 2017 - 2021 bzt (bztsrc@gitlab)
	as part of the 'bootboot' repository on GitLab.
	Original file availible at https://gitlab.com/bztsrc/bootboot/-/blob/master/mykernel/c/kernel.c
*/
/* function to display a string, see below */
int puts(const char *s);
#include <stdint.h>

#include <external/bootboot.h>

/* imported virtual addresses, see linker script */
extern BOOTBOOT bootboot;               // see bootboot.h
extern unsigned char environment[4096]; // configuration, UTF-8 text key=value pairs
extern uint8_t fb;                      // linear framebuffer mapped

/******************************************
 * Entry point, called by BOOTBOOT Loader *
 ******************************************/
void _start() {
	/*** NOTE: this code runs on all cores in parallel ***/
	int x, y, s = bootboot.fb_scanline, w = bootboot.fb_width, h = bootboot.fb_height;

	if(s) {
		// cross-hair to see screen dimension detected correctly
		for (y = 0; y < h; y++) {
			*((uint32_t*)(&fb + s * y + (w * 2))) = 0x00FFFFFF;
		}
		for (x = 0; x < w; x++) {
			*((uint32_t*)(&fb + s * (h / 2) + x * 4)) = 0x00FFFFFF;
		}

		// red, green, blue boxes in order
		for (y = 0; y < 20; y++) {
			for (x = 0; x < 20; x++) {
				*((uint32_t*)(&fb + s * (y + 20) + (x + 20) * 4)) = 0x00FF0000;
			}
		}
		for (y = 0; y < 20; y++) {
			for (x = 0; x < 20; x++) {
				*((uint32_t*)(&fb + s * (y + 20) + (x + 50) * 4)) = 0x0000FF00;
			}
		}
		for (y = 0; y < 20; y++) {
			for (x = 0; x < 20; x++) {
				*((uint32_t*)(&fb + s * (y + 20) + (x + 80) * 4)) = 0x000000FF;
			}
		}

		// say hello
		puts("Hello from a simple BOOTBOOT kernel");
	}
	// hang for now
	while(1);
}

/**************************
 * Display text on screen *
 **************************/
typedef struct {
	uint32_t magic;
	uint32_t version;
	uint32_t headersize;
	uint32_t flags;
	uint32_t numglyph;
	uint32_t bytesperglyph;
	uint32_t height;
	uint32_t width;
	uint8_t glyphs;
} __attribute__((packed)) psf2_t;
extern volatile unsigned char _binary_font_psf_start;

int puts(const char *s) {
	uint64_t len;
	for (len = 0; s[len] != 0; len++);
	psf2_t *font = (psf2_t*)&_binary_font_psf_start;
	int x, y, kx = 0, line, mask, offs;
	int bpl = (font->width + 7) / 8;
	while(*s) {
		unsigned char *glyph = (unsigned char*)&_binary_font_psf_start + font->headersize + (*s > 0 && *s < font->numglyph ? *s : 0) *font->bytesperglyph;
		offs = (kx * (font->width + 1) * 4);
		for (y = 0; y < font->height; y++) {
			line = offs;
			mask = 1 << (font->width - 1);
			for(x = 0; x < font->width; x++) {
				*((uint32_t*)((uint64_t)&fb + line)) = ((int)*glyph) & (mask) ? 0xFFFFFF : 0;
				mask >>= 1;
				line += 4;
			}
			*((uint32_t*)((uint64_t)&fb+line)) = 0;
			glyph += bpl;
			offs += bootboot.fb_scanline;
		}
		s++;
		kx++;
	}
	return len;
}

