#include <stdint.h>
#include <stdbool.h>

uint8_t valueToSet;
extern void KBD_StartSetLED();
extern uint8_t KBD_GetAll();

// FIXME: .... wtf is this
void KBD_SetLED(uint_fast8_t led, bool value) {
	valueToSet = KBD_GetAll();
	if (value) {
		// Enable
		valueToSet |= 1 << led;
	}
	else {
		// Disable
		valueToSet &= ~(1 << led);
	}
	// keyboardStartSetLED();
	asm (
		"mov $0x60, %%dx\n"
		"mov %0, %%al\n"
		// "mov $0x07, %ax\n"
		"outb %%al, %%dx\n"
		"call KBD_WaitForACK\n"
		: "=r" (valueToSet)
		:
	);
	valueToSet = 7;
}