#include "../util.h"
void putc(const char c, void *stream);
void putchar(char c) {
	// Literally just calls putc with the stdout steam
	putc(c, stdout);
}
