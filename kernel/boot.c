#include <kernel/custom.h>
#include <kernel/misc.h>
#include <kernel/panic.h>
#include <kernel/graphics.h>
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
	putcAt('#', __bootProgressBarX + 1 + i, __bootProgressBarY, vga.colors.lgray - 0x101010);
}