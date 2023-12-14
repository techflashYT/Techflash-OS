// NOTE: Yes, this is indeed the same file between both the internal libc and the userspace wrappers, I just symlinked it across.
#include <stdio.h>
int puts(const char *s) {
	/*
		Each iteration of this while loop prints a single character.
	*/
	while (*s) {
		putchar(*s);
		s++;
	}
	putchar('\r');
	putchar('\n');
	return 0;
}
