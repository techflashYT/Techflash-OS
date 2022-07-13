#ifndef KERNEL_HARDWARE_KBD_H
#define KERNEL_HARDWARE_KBD_H

#include <stdint.h>
#include <stdbool.h>
#define KEYBOARD_LED_NUMLOCK 6
#define KEYBOARD_LED_CAPSLOCK 5
#define KEYBOARD_LED_SCROLLLOCK 7

typedef struct {
	bool (*getStatusLED)(uint8_t led);
	void (*setLED)(uint8_t led, bool value);
} __keyboard_t;
extern __keyboard_t keyboard;
#endif