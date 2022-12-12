#include <stdint.h>
#include <stdbool.h>
#include <kernel/tty.h>
#include <kernel/graphics.h>
#include <kernel/custom.h>

// FIXME: The cursor doesn't get erased properly
bool blink = false;
bool needsToEraseBlink = true;
uint16_t origXPos = 0;
uint16_t origYPos = 0;
extern uint32_t PITTick;
uint16_t shouldWait = 0;
extern char* lastNextKey;
void kernTTY_blink() {
	if (shouldWait != 0) {
		needsToEraseBlink = true;
		origXPos = kernTTY.cursorX;
		origYPos = kernTTY.cursorY;
		putcAt('_', origXPos, origYPos, vga.colors.lgray);
		shouldWait--;
		if (lastNextKey[0] == '\b') {
			putcAt(' ', origXPos + 1, origYPos, vga.colors.lgray);
		}
		else if (lastNextKey[0] == '\r') {
			putcAt(' ', origXPos, origYPos, vga.colors.lgray);
		}
		return;
	}
	
	if (kernTTY.blinkingCursor) {
		needsToEraseBlink = true;
		if (kernTTY.nextBlinkShouldBeOn) {
			shouldWait = 250;
			origXPos = kernTTY.cursorX;
			origYPos = kernTTY.cursorY;
			kernTTY.nextBlinkShouldBeOn = false;
			blink = true;
			putcAt('_', origXPos, origYPos, vga.colors.lgray);
			return;
		}
		
		if ((PITTick % 250) == 0) {
			if (!blink) {
				origXPos = kernTTY.cursorX;
				origYPos = kernTTY.cursorY;
				blink = true;
				putcAt('_', origXPos, origYPos, vga.colors.lgray);
			}
			else {
				blink = false;
				if (origYPos != kernTTY.cursorY) {
					putcAt(' ', origXPos, origYPos, vga.colors.lgray);
				}
				if (origYPos == kernTTY.cursorY) {
					if (origXPos >= kernTTY.cursorX) {
						putcAt(' ', origXPos, origYPos, vga.colors.lgray);
					}
				}
			}
		}
	}
	else {
		if (needsToEraseBlink) {
			needsToEraseBlink = false;
			putcAt(' ', kernTTY.cursorX, kernTTY.cursorY, vga.colors.lgray);
		}
	}
}