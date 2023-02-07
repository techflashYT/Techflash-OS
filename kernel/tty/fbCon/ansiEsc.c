#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <kernel/tty/tty.h>
#define STATE_READY 0
#define STATE_BUILDING 1
extern bool nextCharIsEsc;
char builtUpEscCode[8] = { 0 };
uint8_t fbConEscState = 0;
uint8_t escIndex = 0;
bool fbConHandleEsc(char nextChar) {
	switch (fbConEscState) {
		case STATE_READY: {
			fbConEscState = STATE_BUILDING; // remember that we have begun building it
			memset(builtUpEscCode, 0, 8); // clear out any old junk from previous attempts
			escIndex = 0; // reset the index from any previous attempts
			__attribute__((fallthrough)); // intentionally fall through
		}
		case STATE_BUILDING: {
			if (escIndex == 8) {
				fbConEscState = STATE_READY;
				return false; // we've been reading too long, give up
			}
			builtUpEscCode[escIndex] = nextChar;
			escIndex++;
			goto validate;
		}
	}
validate:
	bool isTFOSEsc = false;
	(void)isTFOSEsc;
	bool escStarted = false;
	for (uint8_t i = 0; i != escIndex; i++) {
		char nextByte = builtUpEscCode[i];
		switch (nextByte) {
			case ' ': {
				// whitespace, ignore it
				memmove(builtUpEscCode + i, builtUpEscCode + i + 1, 8 - i);
				i--;
				break;
			}
			case 'T': {
				// Techflash OS specific escape code
				isTFOSEsc = true;
				break;
			}
			case '[': {
				escStarted = true;
				break;
			}
			case 'H': {
				if (escStarted) {
					// move cursor to 0, 0
					kernTTY.cursorX = 0;
					kernTTY.cursorAfterPromptX = 0;
					kernTTY.cursorY = 0;
					fbConEscState = STATE_READY;
					return true;
				}
			}
		}
	}
	return false;
}