#include <stdint.h>
int strcmp(const char *str1, const char *str2) {
	while (*str1) {
		// if characters differ, or end of the second string is reached
		if (*str1 != *str2) {
			break;
		}
 
		// move to the next pair of characters
		str1++;
		str2++;
	}
 
	// return the ASCII difference after converting `char*` to `unsigned char*`
	return *(const uint8_t *)str1 - *(const uint8_t *)str2;
}