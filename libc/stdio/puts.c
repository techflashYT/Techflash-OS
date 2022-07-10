#include <stdint.h>
#include <stdio.h>
#include <kernel/font.h>
#include <kernel/environment.h>
#include <kernel/tty.h>

int puts(const char *s) {
	uint64_t len;
	for (len = 0; s[len] != 0; len++);

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
