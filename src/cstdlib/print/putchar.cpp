#include "../util.hpp"
#include "../../shared.hpp"
void putchar(char c, uint8_t x = 255, uint8_t y = 255) {
	if (x == 255 && y == 255) {
		x = terminal.cursor.position.x;
		y = terminal.cursor.position.y;
	}
	vga_buffer[x + (y * VGA_WIDTH)] = c;
}
