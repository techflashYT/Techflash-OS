#include <stdint.h>
#include "../../../../inc/util.hpp"
void kernelPanic(const char message[]);
struct KeyboardScancode {
    uint_fast8_t scanCode;
    char charValue;
};


struct Keyboard {
    static const KeyboardScancode regularKeyScanCodes[];
	char buffer[256];
	uint_fast8_t bufferIndex = 0;

	void bufferPush(char value) {
		if (bufferIndex != 255) {
			buffer[bufferIndex++] = value;
		}
		else {
			kernelPanic("Keyboard buffer overflow!");
		}
	}
	char bufferPop() {
		if (bufferIndex > 0) {
			return buffer[bufferIndex--];
		}
		else {
			return 0; // Buffer empty
		}
	}
	char convert(uint_fast8_t character) {
		for (int_fast8_t i = 0;regularKeyScanCodes[i].scanCode!=0x00;i++) {
			if (regularKeyScanCodes[i].scanCode == character) {
				return regularKeyScanCodes[i].charValue;
			}
			return 0;
		}
        return 0;
	}
	void handler() {
		uint_fast8_t scancode = inb(0x60);
		bufferPush(convert(scancode));
	}
	const char* getChar() {
		char* ret = 0;
		while (!ret) {
			*ret = bufferPop();
		}
        return ret;
	}
};

const KeyboardScancode Keyboard::regularKeyScanCodes[] = {
	// This will be put in the layout of a full size QWERTY keyboard for convenience of viewing.
	{0x29, '`'}, {0x02, '1'}, {0x03, '2'}, {0x04, '3'}, {0x05, '4'}, {0x06, '5'}, {0x07, '6'}, {0x08, '7'}, {0x09, '8'}, {0x0A, '9'}, {0x0B, '0'}, {0x0C, '-'}, {0x0D, '='}, {0x0E, '\b'/*Backspace*/},
	{0x0F, '\t'/*Tab*/}, {0x10, 'q'}, {0x11, 'w'}, {0x12, 'e'}, {0x13, 'r'}, {0x14, 't'}, {0x15, 'y'}, {0x16, 'u'}, {0x17, 'i'}, {0x18, 'o'}, {0x19, 'p'}, {0x1A, '['}, {0x1B, ']'}, {0x2B, '\\'},
	{0x1E, 'a'}, {0x1F, 's'}, {0x20, 'd'}, {0x21, 'f'}, {0x22, 'g'}, {0x23, 'h'}, {0x24, 'j'}, {0x25, 'k'}, {0x26, 'l'}, {0x27, ';'}, {0x28, '\''}, /* 0x1C is Enter */
	/* 0x2A is LShift */ {0x2C, 'z'}, {0x2D, 'x'}, {0x2E, 'c'}, {0x2F, 'v'}, {0x30, 'b'}, {0x31, 'n'}, {0x32, 'm'}, {0x33, ','}, {0x34, '.'}, {0x35, '/'}, /* 0x36 is RShift */
	/* 0x1D is LControl */ // TODO Rest of keys here https://wiki.osdev.org/PS/2_Keyboard#Scan_Code_Set_1
};
