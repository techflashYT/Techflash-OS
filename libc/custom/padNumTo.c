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
	// Yes I know this is very.  VERY stupid.  But it technically accounts for every possibility of the number of padding.
	// FIXME: Cppcheck complains that it's undefined behavior, yet GCC doesn't even spit out a warning.  Find out why.
	for (size_t i = 0; i < padding - len; i++) {
		dest[i] = '0';
	}

	// Copy the src string into the dest string after the padding.
	memcpy(dest + padding - len, src, len);
	// Null terminate
	dest[padding] = '\0';
	return dest;
}