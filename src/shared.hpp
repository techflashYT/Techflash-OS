#pragma once
#include "cstdlib/util.hpp"

#define u __attribute__ ((unused)) 

#define VERSION "0.0.1a"

#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define PURPLE 5
#define BROWN 6
#define LIGHT_GRAY 7
#define DARK_GRAY 8
#define LIGHT_BLUE 9
#define LIGHT_GREEN 10
#define LIGHT_CYAN 11
#define LIGHT_RED 12
#define LIGHT_PURPLE 13
#define LIGHT_BROWN 14
#define WHITE 15

#define VGA_HEIGHT 25
#define VGA_WIDTH 80


u uint8_t inb(uint16_t port);
u void outb(uint16_t port, uint8_t val);
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

u static uint16_t* vga_buffer = (uint16_t*)0xB8000;
