#include "../util.h"
#include "../../shared.hpp"
extern "C" {
	void putc(const char c, void *stream);
}
void putc(const char c, void *stream) {
	(void)stream;
	vga_buffer[terminal.cursor.position.x + terminal.cursor.position.y * VGA_WIDTH] = c;
}
