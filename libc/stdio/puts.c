#include <stdio.h>
int puts(const char *s) {
	/*
		Each iteration of this while loop prints a single character.
	*/
	int len = 0;
	while (*s) {
		putchar(*s);
		s++;
		len++;
		// kx++;
	}
	return len;
}
