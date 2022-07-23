#include <stdint.h>
#include <stdio.h>
#include <kernel/font.h>
#include <kernel/environment.h>
#include <kernel/tty.h>

int puts(const char *s) {
	uint64_t len = 0;
	for (; s[len] != 0; len++);
	if (len == 0) {
		return 0;
	}

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
