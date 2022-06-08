#include <stddef.h>
char *strcpy(char *dest, const char *src) {
	for (size_t i = 0; src[i] != '\0'; i++) {
		dest[i] = src[i];
	}
	return dest;
}
