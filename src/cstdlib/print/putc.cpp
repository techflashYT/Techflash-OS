void putc(const char *c, void *stream, uint8_t x = 255, uint8_t y = 255) {
	vga_buffer[((x == 255) ? terminal.position.x : x) + (((y == 255) ? terminal.position.y : y) * VGA_WIDTH)] = c;
}