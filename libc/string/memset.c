#include <stdint.h>
void *memset(void *b, int c, int len) {
	uint8_t *p = b;
	while (len > 0) {
		*p = c;
		p++;
		len--;
	}
	return b;
}
