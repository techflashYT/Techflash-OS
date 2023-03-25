#include <stdint.h>
#include <stddef.h>
#include <string.h>

void *memmove(void *dest, const void *src, size_t n) {
	unsigned char* dst = (unsigned char*) dest;
	const unsigned char* srcTemp = (const unsigned char*) src;
	if (dst < srcTemp) {
		for (size_t i = 0; i < n; i++)
			dst[i] = srcTemp[i];
	} else {
		for (size_t i = n; i != 0; i--)
			dst[i-1] = srcTemp[i-1];
	}
	return dest;
}