#include <stdlib.h>
void *memcpy(void* destination, const void* source, size_t num) {
	// Convert to char* for easy access
	char *src  = (char *)source;
	char *dest = (char *)destination;

	// Copy bytes
	for (size_t i = 0; i < num; i++) {
		dest[i] = src[i];
	}
	return destination;
}