#include "../util.h"
#include "../../shared.hpp"
void putc(const char c, void *stream);
uint8_t fputs(const char *str, void *stream) {
	(void)stream;
	uint8_t i = 0;
	for (; str[i] != '\0'; i++) {
		if (i + terminal.cursor.position.x >= VGA_WIDTH) {
			terminal.cursor.position.x = 0;
			terminal.cursor.position.y++;
		}
		putc(str[i], stdout);
	}
	return i;
}
