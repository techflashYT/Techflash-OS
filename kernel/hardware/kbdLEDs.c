#include <stdint.h>
#include <stdbool.h>

uint8_t valueToSet;
void keyboardStartSetLED();
uint8_t keyboardGetAll();
bool keyboardGetStatusOfLED(uint8_t led);

void keyboardSetLED(uint8_t led, bool value) {
	valueToSet = keyboardGetAll();
	if (value) {
		// Enable
		valueToSet |= 1 << led;
	}
	else {
		// Disable
		valueToSet &= ~(1 << led);
	}
	// keyboardStartSetLED();
	asm volatile (
		"mov $0x60, %dx\n"
		"mov valueToSet, %al\n"
		// "mov $0x07, %ax\n"
		"outb %al, %dx\n"
		"call keyboardWaitForACK\n"
	);
	valueToSet = 7;
}