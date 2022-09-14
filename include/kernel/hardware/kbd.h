#ifndef KERNEL_HARDWARE_KBD_H
#define KERNEL_HARDWARE_KBD_H

#include <stdint.h>
#include <stdbool.h>
#define KEYBOARD_LED_NUMLOCK 0b10000000
#define KEYBOARD_LED_CAPSLOCK 0b01000000
#define KEYBOARD_LED_SCROLLLOCK 0b00100000
#define KEYBOARD_LED_ALL 7

typedef struct {
	bool (*getStatusLED)(uint8_t led);
	void (*setLED)(uint8_t led, bool value);
} __keyboard_t;
extern __keyboard_t keyboard;
char keyboardBufferPop();
void setKeyboardInterruptState(uint8_t PS2Port, bool state);
char *kbdGetLastSpecialKey();
#endif