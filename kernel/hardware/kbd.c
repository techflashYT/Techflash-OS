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
#include <string.h>
#include <kernel/tty.h>
char keyboardBuffer[256];
uint8_t keyboardBufferCurrent = 0;
bool isShifted = false;
char *nextKey;
char *lastNextKey;
char shiftedScancodes[] = {0xFF,
	0xFF, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 0xFF,
	0xFF, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', 0xFF,
	0xFF, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':','\"', '~', 0xFF
};
char scancodes[] = {0xFF,
	0xFF, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0xFF,
	0xFF, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0xFF,
	0xFF, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';','\'', '`', 0xFF
};
char kbdScancodeToASCII(uint8_t scancode) {
	char key;
	if (scancode > 0x0F) {
		return 0x00;
	}
	if (!isShifted) {
		key = shiftedScancodes[scancode];
	}
	else {
		key = scancodes[scancode];
	}
	if ((uint8_t)key == (uint8_t)0xFF) {
		if (scancode == 0x00) {
			return 0x0;
		}
		else if (scancode == 0x01) {
			nextKey = "ESC";
			return 0xFF;
		}
		// Backspace
		else if (scancode == 0x0E) {
			kernTTY.cursorX--;
			putchar(' ');
			kernTTY.cursorX--;
			return 0x0;
		}
		else if (scancode == 0x0F) {
			nextKey = "Tab";
			return 0xFF;
		}
		else if (scancode == 0x1C) {
			nextKey = "Ent";
			return 0xFF;
		}
		else if (scancode == 0x1D) {
			nextKey = "LCt";
			return 0xFF;
		}
		else if (scancode == 0x36) {
			isShifted = true;
			return 0x00;
		}
	}
	return key;
}
char *kbdGetLastSpecialKey() {
	lastNextKey = nextKey;
	nextKey = "\0\0\0";
	return lastNextKey;
}
char keyboardBufferPop() {
	if (keyboardBufferCurrent == 0) {
		return '\0';
	}
	char ret = keyboardBuffer[keyboardBufferCurrent];
	keyboardBuffer[keyboardBufferCurrent] = '\0';
	keyboardBufferCurrent--;
	return ret;
}
void keyboardIRQ(registers_t *regs) {
	int scancode = 0;
	int key = 0;
	for (uint16_t i = 0; i < 1000; i++) {
        // Check if scancode is ready
        if ((inb(0x64) & 1) == 0) {
			continue;
		}
        // Get the scan code
        scancode = inb(0x60);
        break;
    }
	key = kbdScancodeToASCII(scancode);
	kernTTY.cursorX = 0;
	kernTTY.cursorY = 5;
	// printf("Keyboard IRQ!  Key: %c\r\nScancode: 0x%x  \r\n", key, scancode);
	keyboardBuffer[keyboardBufferCurrent++] = key;
}
void keyboardInit() {
	nextKey = "\0\0\0";
	memset(keyboardBuffer, '\0', 256);
	registerInterruptHandler(IRQ1, &keyboardIRQ);
}
void setKeyboardInterruptState(uint8_t PS2Port, bool state) {
	if (PS2Port == 0) {
		outb(0x64, (uint8_t)(0xA7 + state));
	}
	else {
		DUMPREGS
		panic("setKeyboardInterruptState(): Called with invalid PS2 Port number\r\n       or with port 2 which is unsupported", regs);
	}
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