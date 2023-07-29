#include <stdint.h>
#include <stddef.h>
int memcmp(const void *s1, const void *s2, size_t len) {
	const uint8_t *p = s1;
	const uint8_t *q = s2;
	int charCompareStatus = 0;
	// If both pointer pointing same memory block
	if (s1 == s2) {
		return charCompareStatus;
	}
	while (len > 0) {
		if (*p != *q) {
			// compare the mismatching character
			charCompareStatus = (*p > *q) ? 1 : -1;
			break;
		}
		len--;
		p++;
		q++;
	}
	return charCompareStatus;
}