#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H
#include <stdbool.h>
#include <stdint.h>
typedef struct {
	bool ready;
	uint16_t cursorX;	
	uint16_t cursorY;
	uint32_t color;
	const char* buffer;
	void (*init)();
} __attribute__((packed)) _kernTTY_t;
extern _kernTTY_t kernTTY;
#endif