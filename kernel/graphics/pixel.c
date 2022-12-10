#include <stdint.h>
#include <kernel/environment.h>
uint32_t readPixel(const uint16_t x, const uint16_t y) {
	uint32_t* pixel = (uint32_t*)(&fb + (y * bootboot.fb_width + x) * 2);
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Warray-bounds"
	return *pixel;
	#pragma GCC diagnostic pop
}
void putPixel(const uint16_t x, const uint16_t y, const uint32_t value) {
	uint32_t* pixel = (uint32_t*)(&fb + (y * bootboot.fb_width + x) * 2);
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Warray-bounds"
	*pixel = value;
	return;
	#pragma GCC diagnostic pop
}