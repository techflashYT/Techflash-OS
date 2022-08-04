#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <kernel/font.h>
#include <kernel/environment.h>
#include <kernel/tty.h>

int puts(const char *s) {
	uint64_t len = strlen(s);

	/*
		Each iteration of this while loop prints a single character.
	*/
	while(*s) {
		putchar(*s);
		s++;
		// kx++;
	}
	return len;
}
