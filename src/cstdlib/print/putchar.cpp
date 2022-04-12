#include "../util.hpp"
#include "../../shared.hpp"
void putc(const char *c, void *stream, const uint8_t &x = 255, const uint8_t &y = 255);
void putchar(char c, uint8_t x = 255, uint8_t y = 255) {
	// Literally just calls putc with the stdout steam
	putc(c, stdout, ((x == 255) ? terminal.position.x : x), ((y == 255) ? terminal.position.y : y));
}
