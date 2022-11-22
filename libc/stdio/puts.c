#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <kernel/font.h>
#include <kernel/environment.h>
#include <kernel/tty.h>

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
