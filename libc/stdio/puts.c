// NOTE: Yes, this is indeed the same file between both the internal libc and the userspace wrappers, I just symlinked it across.
#include <stdio.h>
#include <stdint.h>
// int len = 0;
char buffer[16] = {0};
int puts(const char *s) {
	/*
		Each iteration of this while loop prints a single character.
	*/
	// len = 0;
	if ((*s) == 0) {
		asm (
			"mov $0x01, %%rax;"
			"mov %0, %%rdi;"
			"mov %1, %%rsi;"
			"mov $10,%%r8;"
			"int $0x80;"
			:
			: "r" (s), "r" (buffer)
			:
		);
		putchar('w');
		putchar('e');
		putchar(' ');
		putchar('d');
		putchar('e');
		putchar('d');
		putchar(' ');
		putchar(' ');
		uint8_t i = 0;
		while (buffer[i] != 0) {
			putchar(buffer[i]);
			i++;
		}
	}
	while (*s) {
		putchar(*s);
		s++;
		// len++;
	}
	return 0;
}
