#include <stdint.h>
#include <kernel/tty.h>
#include <kernel/hardware/kbd.h>


extern void __kernTTY_init();
_kernTTY_t kernTTY;

extern bool __keyboardGetStatusOfLED(uint8_t led);
extern void __keyboardSetLED(uint8_t led, bool value);
__keyboard_t keyboard;
void __init() {
	// Start initializing a TTY.
	kernTTY.init = __kernTTY_init;
	kernTTY.init();

	// Start setting up the keyboard struct.
	keyboard.getStatusLED = __keyboardGetStatusOfLED;
	keyboard.setLED = __keyboardSetLED;
}