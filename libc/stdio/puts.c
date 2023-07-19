// NOTE: Yes, this is indeed the same file between both the internal libc and the userspace wrappers, I just symlinked it across.
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <kernel/tty.h>
char buffer[16] = {0};
int puts(const char *s) {
	/*
		Each iteration of this while loop prints a single character.
	*/
	// len = 0;
	/*if ((*s) == 0) {
		putchar('w');
		putchar('e');
		putchar(' ');
		putchar('d');
		putchar('e');
		putchar('d');
		putchar(' ');
		putchar(' ');
		return 0;
	}*/
	flanterm_write(TTY_Ctx, s, strlen(s));
	return 0;
}
