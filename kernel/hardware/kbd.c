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

char lastKey;

bool isShifted = false;

char *nextKey;
char *lastNextKey;

char shiftedScancodes[] = {0xFF,
	0xFF, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 0xFF,
	0xFF, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', 0xFF,
	0xFF, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':','\"', '~', 0xFF,
	'|',  'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?',0xFF, '*', 0xFF,
	' ', 0xFF, 
};

char scancodes[] = {0xFF,
	0xFF, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0xFF,
	0xFF, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0xFF,
	0xFF, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';','\'', '`', 0xFF,
	'\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',0xFF, '*', 0xFF,
	' ', 0xFF,
};

char kbdScancodeToASCII(uint8_t scancode) {
	char key;
	if (scancode > 0x3A) {
		return 0x00;
	}
	if (!isShifted) {
		key = scancodes[scancode];
	}
	else {
		key = shiftedScancodes[scancode];
	}

	if ((uint8_t)key == (uint8_t)0xFF) {
		if (scancode == 0x00) {
			return 0x0;
		}
		else if (scancode == 0x01) {
			strcpy(nextKey, "ESC");
			return 0xFF;
		}
		// Backspace
		else if (scancode == 0x0E) {
			strcpy(nextKey, "\b");
			return 0xFF;
		}
		else if (scancode == 0x0F) {
			strcpy(nextKey, "Tab");
			return 0xFF;
		}
		else if (scancode == 0x1C) {
			strcpy(nextKey, "\r\n");
			return 0xFF;
		}
		else if (scancode == 0x1D) {
			strcpy(nextKey, "LCt");
			return 0xFF;
		}
		else if (scancode == 0x36) {
			isShifted = true;
			return 0x00;
		}
		else if (scancode == 0x3A) {
			keyboard.setLED(KEYBOARD_LED_CAPSLOCK, !keyboard.getStatusLED(KEYBOARD_LED_CAPSLOCK));
			isShifted = true;
		}
	}
	// printf("key: %c", key);
	return key;
}

char *kbdGetLastSpecialKey() {
	lastNextKey = nextKey;
	nextKey = "\0\0\0";
	return lastNextKey;
}


void keyboardIRQ(registers_t *regs) {
	kernTTY.nextBlinkShouldBeOn = true;
	kernTTY.blink();
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
	lastKey = key;
	// kernTTY.cursorX = 0;
	// kernTTY.cursorY = 5;
	// printf("Keyboard IRQ!  Key: %c\r\nScancode: 0x%x  \r\n", key, scancode);
}
char keyboardGetLastKey() {
	char ret = lastKey;
	lastKey = 0;
	return ret;
}

void keyboardInit() {
	nextKey = "\0\0\0";
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
