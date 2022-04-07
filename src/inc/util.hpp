#pragma once
#include "io.hpp"
#include <stdlib.h>
#include <stdint.h>
static inline void halt() {
	for (;;) {
		asm("hlt");
	}
}


static inline void softHalt() {
	asm("hlt");
}



__attribute__((unused)) static uint_fast16_t* vga_buffer = (uint_fast16_t*)0xB8000;


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

static struct Terminal {
	size_t row = 0;
	size_t column = 0;
	size_t width = VGA_WIDTH;
	size_t height = VGA_HEIGHT;
	uint_fast8_t fgColor = LIGHT_GRAY;
	uint_fast8_t bgColor = BLACK;
	struct Cusor {
		void enable(uint_fast8_t start, uint_fast8_t end) {
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
			uint_fast8_t x = 0;
			uint_fast8_t y = 0;
			uint_fast16_t __pos = 0;
			void change(uint_fast8_t posx, uint_fast8_t posy) {
				__pos = posy * VGA_WIDTH + posx;
				x = posx;
				y = posy;
				outb(0x3D4, 0x0F);
				outb(0x3D5, (uint8_t) (__pos & 0xFF));
				outb(0x3D4, 0x0E);
				outb(0x3D5, (uint8_t) ((__pos >> 8) & 0xFF));
			}
		} position;
	} cursor;
} terminal;

void kprintf(int_fast8_t x, uint_fast8_t y, uint_fast8_t color, const int_fast8_t* str, ...);
void kprintf(int_fast8_t x, uint_fast8_t y, uint_fast8_t color, const char* str, ...);

static inline uint_fast32_t strlen(const int_fast8_t *str) {
	int i = 0;
	while (str[i] != (char)0) {
		++i;
	}
	return i;
}
static inline uint_fast32_t strlen(const char *str) {
	int i = 0;
	while (str[i] != (char)0) {
		++i;
	}
	return i;
}


static inline void kernelPanic(const int_fast8_t message[]) {
	kprintf(0, 0, RED, "KERNEL PANIC: %s", message);
}
static inline void kernelPanic(const char message[]) {
	kprintf(0, 0, RED, "KERNEL PANIC: %s", message);
}
// void *memset(void *b, int val, size_t count) {
	// asm volatile ("cld; rep stosb" : "+c" (count), "+D" (b) : "a" (val) : "memory");
	// return b;
// }
