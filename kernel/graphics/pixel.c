#include <stdint.h>
#include <kernel/environment.h>
uint16_t readPixel(const uint16_t x, const uint16_t y) {
	uint16_t* pixel = (uint16_t*)(&fb + (y * bootboot.fb_width + x) * 2);
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Warray-bounds"
	return *pixel;
	#pragma GCC diagnostic pop
}
