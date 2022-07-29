#include <stdio.h>
#include <stdlib.h>
int putsNoTerminator(const char* str, size_t len) {
	while (len--) {
		if (putchar(*str++)) {
			return -1;
		}
	}
	return 0;
}
