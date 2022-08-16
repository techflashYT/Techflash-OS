#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H
#include <stdbool.h>
#include <stdint.h>
typedef struct {
	bool ready;
	uint16_t cursorX;	
	uint16_t cursorY;
	uint32_t color;
	uint32_t textBackground;
	uint32_t width;
	uint32_t height;
	uint32_t index;
	char* buffer;
	void (*init)();
	void (*setBackground)(const uint32_t color);
} __attribute__((packed)) _kernTTY_t;
extern _kernTTY_t kernTTY;
void updateScreen(const char* buffer);
#endif
