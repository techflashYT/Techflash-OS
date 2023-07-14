#include <stdint.h>
#include <stdlib.h>
#include <string.h>
// Pad the number to the given length by adding zeros in front of it.
void padTo(char *src, uint_fast8_t padding) {
	// Get the length of the number.
	size_t len = strlen(src);

	char temp[len + padding];
	if (len >= padding) {
		return;
	}
	for (size_t i = 0; i < padding - len; i++) {
		temp[i] = '0';
	}

	// Copy the src string into the temp string after the padding.
	memcpy(temp + padding - len, src, len);
	// Null terminate
	temp[padding] = '\0';
	strcpy(src, temp);
	return;
}