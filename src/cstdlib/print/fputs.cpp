#include "../util.hpp"
#include "../../shared.hpp"
void putc(const char *c, void *stream, uint8_t x = 255, uint8_t y = 255);
uint8_t fputs(const char *str, void *stream, const uint8_t &x, const uint8_t &y) {
	for (uint8_t i = 0; str[i] != '\0'; i++) {
		// TODO IF MORE THAN VGA WIDTH THEN WRAP
		putc(str[i], stdout, i, )
	}
}
