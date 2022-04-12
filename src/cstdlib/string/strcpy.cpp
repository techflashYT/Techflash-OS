#include "../util.hpp"
// Forward declarations for functions that we end up using here
void *memcpy(void *dest, const void *src, const size_t &count);
size_t strlen(const char *str);

char *strcpy(char *dest, const char *src) {
	size_t len = strlen(src);
	memcpy(dest, src, len + 1);
	return dest;
}
