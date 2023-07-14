#ifndef KERNEL_HARDWARE_KBD_H
#define KERNEL_HARDWARE_KBD_H

#include <stdint.h>
#include <stdbool.h>
#define KEYBOARD_LED_NUMLOCK    0b10000000
#define KEYBOARD_LED_CAPSLOCK   0b01000000
#define KEYBOARD_LED_SCROLLLOCK 0b00100000
#define KEYBOARD_LED_ALL 7


extern void  KBD_Init();
extern uint8_t KBD_GetLED(uint64_t led);
extern void  KBD_SetLED(uint_fast8_t led, bool value);
extern char *KBD_GetLastSpecialKey();
extern char  KBD_GetLastKey();
extern void  KBD_SetIntState(uint_fast8_t PS2Port, bool state);

#endif