#include <stddef.h>
#include <string.h>

char *strstr(const char *haystack, const char *needle) {
	size_t size = strlen(needle);

	while (*haystack) {
		if (!memcmp(haystack, needle, size)) {
			return (char *)haystack;
		}
		haystack++;
	}
	return NULL;
}
