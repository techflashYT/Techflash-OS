#include "../util.hpp"
// Forward declarations for functions that we end up using here
size_t strlen(const char *str);

char *strcat(char *dest, const char *src) {
    size_t i,j;
	i = strlen(src);
    for (j = 0; src[j] != '\0'; j++) {
        dest[i+j] = src[j];
	}
    dest[i+j] = '\0';
    return dest;
}
