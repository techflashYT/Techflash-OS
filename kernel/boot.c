#include <kernel/custom.h>
#include <kernel/misc.h>
#include <kernel/panic.h>
#include <kernel/graphics.h>
#include <kernel/environment.h>
#include <kernel/hardware/serial.h>
#include <kernel/hardware/PIT.h>
uint8_t __bootProgressBarX = 0;
uint8_t __bootProgressBarY = 0;
uint8_t __bootProgressBarWidth = 0;
void __bootProgressBarCreate(const uint8_t x, const uint8_t y, const uint8_t width) {
	// Set global vars so that we know these properties in other functions
	__bootProgressBarX = x;
	__bootProgressBarY = y;
	__bootProgressBarWidth = width;
	
	// Create the progress bar.
	putcAt('[', x, y, vga.colors.lgray);
	putcAt(']', x + width, y, vga.colors.lgray);
	for (uint8_t i = 0; i < (width - 1); i++) {
		putcAt('#', x + 1 + i, y, 0x454550);
	}
}
void __bootProgressBarFadeOut() {
	uint32_t bracketColor = vga.colors.lgray;
	uint32_t hashColor    = vga.colors.white;
	while (true) {
		sleep(3);
		if (hashColor != 0x00000000) {
			hashColor -= 0x00010101;
		}
		if (bracketColor != 0x00000000) {
			bracketColor -= 0x00010101;
		}
		putcAt('[', __bootProgressBarX, __bootProgressBarY, bracketColor);
		putcAt(']', __bootProgressBarX + __bootProgressBarWidth, __bootProgressBarY, bracketColor);
		for (uint8_t i = 0; i < (__bootProgressBarWidth - 1); i++) {
			putcAt('#', __bootProgressBarX + 1 + i, __bootProgressBarY, hashColor);
		}
		if (hashColor == 0 && bracketColor == 0) {
			break;
		}
	}
}
uint8_t __percentToCoords(const uint8_t percent) {
	// Calculate the x coords for the progress bar.
	return (__bootProgressBarWidth - 1) * percent / 100;
}
void __bootProgressBarUpdate(const uint8_t percent) {
	// Verify that the percent is valid.
	if (percent == 0 || percent > 100) {
		DUMPREGS
		panic("__bootProgressBarUpdate(): Percent not within range 1-100", regs);
	}
	// Update the progress bar.
	uint8_t i = 0;
	for (; i < __percentToCoords(percent); i++) {
		putcAt('#', __bootProgressBarX + 1 + i, __bootProgressBarY, vga.colors.white);
	}
	if (percent != 100) {
		putcAt('#', __bootProgressBarX + 1 + i, __bootProgressBarY, vga.colors.lgray - 0x101010);
	}
}