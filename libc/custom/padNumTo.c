#include <stdint.h>
#include <string.h>
char dest[300] = {'a'};
char* padNumTo(const char *src, uint8_t padding) {
	// Pad the number to the given length by adding zeros in front of it.

	// Get the length of the number.
	size_t len = strlen(src);
	if (len >= padding) {
		return (char *)src;
	}
	for (size_t i = 0; i < padding - len; i++) {
		dest[i] = '0';
	}

	// Copy the src string into the dest string after the padding.
	memcpy(dest + padding - len, src, len);
	// Null terminate
	dest[padding] = '\0';
	return dest;
}