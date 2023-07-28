#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>

void *memmove(void *dest, const void *src, size_t n) {
	assert((n != 0));
	assert((dest > (void *)4096));
	assert((src  > (void *)4096));
	assert((dest != src));
	uint8_t* dst = (uint8_t*) dest;
	const uint8_t* srcTemp = (const uint8_t*) src;
	if (dst < srcTemp) {
		for (size_t i = 0; i < n; i++) {
			dst[i] = srcTemp[i];
		}
	}
	else {
		for (size_t i = n; i != 0; i--) {
			dst[i - 1] = srcTemp[i - 1];
		}
	}
	return dest;
}