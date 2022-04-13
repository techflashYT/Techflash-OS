#include "../util.h"
// Forward declarations for functions that we end up using here
void *memcpy(void *dest, const void *src, size_t count);
size_t strlen(const char *str);

char *strcpy(char *dest, const char *src) {
	memcpy(dest, src, strlen(src) + 1);
	return dest;
}
