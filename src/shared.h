#pragma once
#include "cstdlib/util.h"

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
#ifdef __cplusplus
extern "C" {
#endif
	u uint8_t inb(uint16_t port);
	u void outb(uint16_t port, uint8_t val);
#ifdef __cplusplus
}
#endif

u static uint16_t* vga_buffer = (uint16_t*)0xB8000;

#define KERNEL_STACK_SIZE 0x8000
#define KERNEL_HEAP_END 0x02000000
