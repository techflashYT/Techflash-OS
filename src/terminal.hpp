#include "shared.h"
static struct Terminal {
	size_t row = 0;
	size_t column = 0;
	size_t width = VGA_WIDTH;
	size_t height = VGA_HEIGHT;
	uint8_t fgColor = LIGHT_GRAY;
	uint8_t bgColor = BLACK;
	struct Cusor {
		void enable(uint8_t start, uint8_t end) {
			outb(0x3D4, 0x0A);
			outb(0x3D5, (inb(0x3D5) & 0xC0) | start);

			outb(0x3D4, 0x0B);
			outb(0x3D5, (inb(0x3D5) & 0xE0) | end);
		}
		void disable() {
			outb(0x3D4, 0x0A);
			outb(0x3D5, 0x20);
		}
		struct Position {
			uint8_t x = 0;
			uint8_t y = 0;
			void change(uint8_t posx, uint8_t posy) {
				x = posx;
				y = posy;
				outb(0x3D4, 0x0F);
				outb(0x3D5, (uint8_t) ((x + (y * VGA_WIDTH)) & 0xFF));
				outb(0x3D4, 0x0E);
				outb(0x3D5, (uint8_t) (((x + (y * VGA_WIDTH)) >> 8) & 0xFF));
			}
		} position;
	} cursor;
} terminal;
