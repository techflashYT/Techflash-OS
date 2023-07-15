#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <kernel/hardware/kbd.h>
#include <kernel/hardware/CPU/x86Setup.h>
#include <kernel/hardware/IO.h>
#include <kernel/panic.h>
static char lastKey;

static bool isShifted = false;

static char nextKey[8];
static char lastNextKey[8];

static char shiftedScancodes[] = {0xFF,
	0xFF, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 0xFF,
	0xFF, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', 0xFF,
	0xFF, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':','\"', '~', 0xFF,
	'|',  'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?',0xFF, '*', 0xFF,
	' ', 0xFF, 
};

static char scancodes[] = {0xFF,
	0xFF, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0xFF,
	0xFF, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0xFF,
	0xFF, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';','\'', '`', 0xFF,
	'\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',0xFF, '*', 0xFF,
	' ', 0xFF,
};
static uint8_t convertArr[] = {
	[0x00] = 0,
	[0x01] = 1,
	[0x0E] = 2,
	[0x0F] = 3,
	[0x1C] = 4,
	[0x1D] = 5,
	[0x36] = 6,
	0, // fill everything not set with 0
};
static char *keyStrs[] = {
	0,
	"ESC",
	"\b",
	"Tab",
	"\r\n",
	"LCtrl"
};
static char KBD_ScancodeToASCII(uint8_t scancode) {
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
			KBD_SetLED(KEYBOARD_LED_CAPSLOCK, !KBD_GetLED(KEYBOARD_LED_CAPSLOCK));
			isShifted = true;
		}
	}
	return key;
}

char *KBD_GetLastSpecialKey() {
	memcpy(lastNextKey, nextKey, 8);
	nextKey[0] = '\0';
	return lastNextKey;
}


static void KBD_IRQ(__attribute__ ((unused)) registers_t *regs) {
	// TTY_Blink();
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
	
	key = KBD_ScancodeToASCII(scancode);
	lastKey = key;
}
char KBD_GetLastKey() {
	char ret = lastKey;
	lastKey = 0;
	return ret;
}
void KBD_SetIntState(uint_fast8_t PS2Port, bool state) {
	if (PS2Port == 0) {
		outb(0x64, (uint8_t)(0xA7 + state));
	}
	else {
		DUMPREGS
		panic("KBD_SetIntState(): Called with invalid PS2 Port number\r\n       or with port 2 which is unsupported", regs);
	}
}
void KBD_Init() {
	registerInterruptHandler(0x21, &KBD_IRQ); // register PS2 keyboard handler on IRQ1
}

