#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <kernel/hardware/kbd.h>
#include <kernel/hardware/serial.h>
#include <kernel/hardware/CPU/ISR.h>
#include <kernel/hardware/CPU/IRQNums.h>
#include <kernel/hardware/IO.h>
#include <kernel/panic.h>
#include <kernel/misc.h>
#include <stdint.h>
char keyboardBuffer[256];
uint8_t keyboardBufferCurrent = 0;
char kbdScancodeToASCII(uint8_t scancode) {
	if (scancode == 0) {
		DUMPREGS;
		panic("oh shit keyboard is bad", regs);
	}
	return 'a';
}
void keyboardIRQ(registers_t regs) {
	int scancode = 0;
	for (uint16_t i = 0; i < 1000; i++) {
        // Check if scancode is ready
        if ((inb(0x64) & 1) == 0) {
			continue;
		}
        // Get the scan code
        scancode = inb(0x60);
        break;
    }
	keyboardBuffer[keyboardBufferCurrent++] = kbdScancodeToASCII(scancode);
}
void keyboardInit() {
	registerInterruptHandler(IRQ1, &keyboardIRQ);
}
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