#include <stdint.h>
#include <stddef.h>

// Thanks to https://github.com/Mathewnd for this code!
char *utoa(uint_fast64_t value, volatile char* result, uint_fast8_t base) {
	char buff[sizeof(uint_fast64_t) * 8 / 4];

	if (base < 2 || base > 36) {
		*result = '\0';
		return (char *)result;
	}

	size_t charc = 0;

	while (value) {
		uintmax_t offset = value % base;
		value /= base;
		buff[charc++] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"[offset];
	}
	
	for (uintmax_t i = 0; i < charc; ++i) {
		result[i] = buff[charc - i - 1];
	}

	result[charc] = '\0';
	
	return (char *)result;
}

