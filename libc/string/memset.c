#include <stdint.h>
void *memset(void *s, int c, int len) {
	uint8_t *p = s;
	while (len > 0) {
		*p = c;
		p++;
		len--;
	}
	return s;
}
