#include <stdint.h>
#include <stdbool.h>
#include <kernel/tty.h>
#include <kernel/graphics.h>

// FIXME: The cursor doesn't get erased properly
bool blink = false;
bool needsToEraseBlink = true;
uint16_t x = 0;
uint16_t y = 0;
extern uint32_t PITTick;
uint16_t shouldWait = 0;
extern char* lastNextKey;
void __kernTTY_blink() {
	if (shouldWait != 0) {
		needsToEraseBlink = true;
		x = kernTTY.cursorX;
		y = kernTTY.cursorY;
		putcAt('_', x, y, vga.colors.lgray);
		shouldWait--;
		if (lastNextKey[0] == '\b') {
			putcAt(' ', x + 1, y, vga.colors.lgray);
		}
		else if (lastNextKey[0] == '\r') {
			putcAt(' ', x, y, vga.colors.lgray);
		}
		return;
	}
	
	if (kernTTY.blinkingCursor) {
		needsToEraseBlink = true;
		if (kernTTY.nextBlinkShouldBeOn) {
			shouldWait = 250;
			x = kernTTY.cursorX;
			y = kernTTY.cursorY;
			kernTTY.nextBlinkShouldBeOn = false;
			blink = true;
			putcAt('_', x, y, vga.colors.lgray);
			return;
		}
		
		if ((PITTick % 250) == 0) {
			if (!blink) {
				x = kernTTY.cursorX;
				y = kernTTY.cursorY;
				blink = true;
				putcAt('_', x, y, vga.colors.lgray);
			}
			else {
				blink = false;
				if (y != kernTTY.cursorY) {
					putcAt(' ', x, y, vga.colors.lgray);
				}
				if (y == kernTTY.cursorY) {
					if (x >= kernTTY.cursorX) {
						putcAt(' ', x, y, vga.colors.lgray);
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