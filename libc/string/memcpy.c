#include <stdlib.h>
#include <assert.h>
void *memcpy(void *dest, const void *src, size_t n) {
	assert((n != 0));
	assert((dest > (void *)4096));
	assert((src  > (void *)4096));
	assert((dest != src));

	char *d = dest;
	const char *s = src;
	// Copy bytes
	for (size_t i = 0; i < n; i++) {
		d[i] = s[i];
	}
	return d;
}
