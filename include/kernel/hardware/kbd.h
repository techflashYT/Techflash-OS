#ifndef KERNEL_HARDWARE_KBD_H
#define KERNEL_HARDWARE_KBD_H

#include <stdint.h>
#include <stdbool.h>
#define KEYBOARD_LED_NUMLOCK    0b10000000
#define KEYBOARD_LED_CAPSLOCK   0b01000000
#define KEYBOARD_LED_SCROLLLOCK 0b00100000
#define KEYBOARD_LED_ALL 7

typedef struct {
	bool  (*getStatusLED)     (uint_fast8_t led);
	void  (*setLED)           (uint_fast8_t led, bool value);
	void  (*setIntState)      (uint_fast8_t PS2Port, bool state);
	char  (*getLastKey)       ();
	char *(*getLastSpecialKey)();
} keyboard_t;
extern keyboard_t keyboard;
#endif