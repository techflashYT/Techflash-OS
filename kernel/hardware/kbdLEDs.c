#include <stdint.h>

uint8_t valueToSet;
void __keyboardStartSetLED();
uint8_t __keyboardGetAll();
bool __keyboardGetStatusOfLED(uint8_t led);

void __keyboardSetLED(uint8_t led, bool value) {
	valueToSet = __keyboardGetAll();
	if (value) {
		// Enable
		valueToSet |= 1 << led;
	}
	else {
		// Disable
		valueToSet &= ~(1 << led);
	}
	// __keyboardStartSetLED();
	asm volatile (
		"mov $0x60, %dx\n"
		"mov valueToSet, %al\n"
		// "mov $0x07, %ax\n"
		"outb %al, %dx\n"
		"call __keyboardWaitForACK\n"
	);
	valueToSet = 7;
}