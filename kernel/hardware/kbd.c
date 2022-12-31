#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <kernel/hardware/kbd.h>
#include <kernel/hardware/CPU/ISR.h>
#include <kernel/hardware/CPU/IRQNums.h>
#include <kernel/hardware/IO.h>
#include <kernel/panic.h>
#include <kernel/misc.h>
keyboard_t keyboard;
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
uint8_t convertArr[] = {
	[0x00] = 0x00,
	[0x01] = 0x01,
	[0x0E] = 0x02,
	[0x0F] = 0x03,
	[0x1C] = 0x04,
	[0x1D] = 0x05,
	[0x36] = 0x06,
	0, // fill everything not set with 0
};
char *keyStrs[] = {
	0,
	"ESC",
	"\b",
	"Tab",
	"\r\n",
	"LCtrl"
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
		char *tmp;
		tmp = keyStrs[convertArr[scancode]];
		if (tmp != 0) {
			strcpy(nextKey, tmp);
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
	return key;
}

char *keyboardGetLastSpecialKey() {
	lastNextKey = nextKey;
	nextKey = "\0\0\0";
	return lastNextKey;
}


void keyboardIRQ(__attribute__ ((unused)) registers_t *regs) {
	// kernTTY.nextBlinkShouldBeOn = true;
	// kernTTY.blink();
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
}
char keyboardGetLastKey() {
	char ret = lastKey;
	lastKey = 0;
	return ret;
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
void keyboardSetLED(uint8_t led, bool value);
bool keyboardGetStatusOfLED(uint8_t led);
// false = init struct on keyboardInit
// true  = init IRQ on keyboardInit
bool initStruct = false;
void keyboardInit() {
	if (!initStruct) {
		initStruct = true;
		keyboard.getStatusLED      = keyboardGetStatusOfLED;
		keyboard.setLED            = keyboardSetLED;
		keyboard.setIntState       = setKeyboardInterruptState;
		keyboard.getLastKey        = keyboardGetLastKey;
		keyboard.getLastSpecialKey = keyboardGetLastSpecialKey;
		return;
	}
	nextKey = malloc(8);
	registerInterruptHandler(IRQ1, &keyboardIRQ);
}

