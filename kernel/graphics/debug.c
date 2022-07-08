#include <stdint.h>
#include <kernel/environment.h>
void gfxDebug(int s, int w, int h) {
	int x, y;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
#pragma GCC diagnostic ignored "-Wstringop-overflow"
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
#pragma GCC diagnostic pop
}