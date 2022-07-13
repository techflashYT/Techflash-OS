#include <stdbool.h>
#include <kernel/io.h>
#include <kernel/hardware/kbd.h>
// This is an internal function, won't be exposed globally via the keyboard struct
void __keyboardStartSetLED() {
	outb(0x60, 0xED);
}
// This is an internal function, won't be exposed globally via the keyboard struct
uint8_t __keyboardGetAll() {
	__keyboardStartSetLED();
	return inb(0x60);
}
bool __keyboardGetStatusOfLED(uint8_t led) {
	return ((__keyboardGetAll() >> led) & 1);
}
void __keyboardSetLED(uint8_t led, bool value) {
	uint8_t valueToSet = __keyboardGetAll();
	if (value) {
		// Enable
		valueToSet |= 1 << led;
	}
	else {
		// Disable
		valueToSet &= ~(1 << led);
	}
	__keyboardStartSetLED();
	outb(0x60, valueToSet);
}