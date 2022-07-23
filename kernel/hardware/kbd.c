#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <kernel/hardware/kbd.h>
#include <kernel/hardware/serial.h>
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
	// cppcheck-suppress legacyUninitvar
	char *buffer = itoa(valueToSet, buffer, 10);
	serial.writeString(SERIAL_PORT_COM1, "[    KBRD] valueToSet: ");
	serial.writeString(SERIAL_PORT_COM1, buffer);
	serial.writeString(SERIAL_PORT_COM1, "\r\n");
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