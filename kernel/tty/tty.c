#include <kernel/tty.h>
#include <kernel/font.h>
#include <kernel/environment.h>
#include <external/bootboot.h>
#include <stdbool.h>
void __kernTTY_init() {
	psf2_t *font = (psf2_t*)&_binary_font_psf_start;
	kernTTY.ready = false;
	kernTTY.cursorX = 0;
	kernTTY.cursorY = 0;
	kernTTY.color = 0xAAAAAA; // VGA light gray.
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Warray-bounds"
	kernTTY.width = (bootboot.fb_width / font->width);
	kernTTY.height = (bootboot.fb_height / font->height);
	#pragma GCC diagnostic pop
}