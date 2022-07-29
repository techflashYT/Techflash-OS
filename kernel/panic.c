#include <stdio.h>
#include <stdint.h>
#include <kernel/environment.h>
#include <kernel/graphics.h>
#include <kernel/tty.h>
void panic(const char* message) {
	int s = bootboot.fb_scanline;
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Warray-bounds"
	#pragma GCC diagnostic ignored "-Wstringop-overflow"
	// Set each pixel of the framebuffer to 0xDD0000 [dark red]
	for (uint32_t y = 0; y < bootboot.fb_height; y++) {
		for (uint32_t x = 0; x < bootboot.fb_width; x++) {
			*((uint32_t*)(&fb + s * y + x * 4)) = 0xDD0000;
		}
	}
	#pragma GCC diagnostic pop
	kernTTY.color = vga.colors.white;
	printf("KERNEL PANIC: %s\r\n", message);
	asm volatile (
		"cli\n"
		"hlt"
	);
}