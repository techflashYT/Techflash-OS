#include <stddef.h>
size_t strnlen_s(const char *str, size_t strsz) {
	const char *s;
	for (s = str; *s && strsz--; ++s);
	return (size_t)(s - str);
}