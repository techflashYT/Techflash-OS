#pragma once
#include "io.hpp"
#include "../shared.hpp"
#include <stdlib.h>
#include <stdint.h>

u static void halt() {
	for (;;) {
		asm(
			"cli\r\n"
			"hlt"
		);
	}
}

u static void softHalt() {
	asm("hlt");
}
// C Standard lib forward declarations of functions that are defined in src/cstdlib

void printf(const char *format, ...);
void *memset(void *b, int val, size_t count);
void *memcpy(void *dest, const void *src, size_t count);
size_t strlen(const char *str);
char *strcpy(char *dest, const char *src);
char *strcat(char *dest, const char *src);
void putchar(char c, uint_fast8_t x = 255, uint_fast8_t y = 255);

u static void kernelPanic(const int_fast8_t message[]) {
	printf("\033[1;31mKERNEL PANIC: %s", message);
	halt();
}
u static void kernelPanic(const char message[]) {
	printf("\033[1;31mKERNEL PANIC: %s", message);
	halt();
}

