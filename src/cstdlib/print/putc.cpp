void putc(const char *c, void *stream, uint8_t x = 255, uint8_t y = 255) {
	if (x == 255 && y == 255) {
		x = terminal.cursor.position.x;
		y = terminal.cursor.position.y;
	}
	vga_buffer[((x == 255) ? terminal.position.x : x) + (((y == 255) ? terminal.position.y : y) * VGA_WIDTH)] = c;
}