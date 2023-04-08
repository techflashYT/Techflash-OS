#include <sys/types.h>
void *memset(void *s, uint_fast64_t c, size_t len) {
	uint8_t *p = s;
	while (len > 0) {
		*p = c;
		p++;
		len--;
	}
	return s;
}
