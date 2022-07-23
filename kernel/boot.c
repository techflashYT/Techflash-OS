#include <kernel/tty.h>
void __bootProgressBarCreate(uint8_t x, uint8_t y, uint8_t width) {
	// Create the progress bar.
	putcAt('[', x, y, 0xAAAAAA);
	putcAt(']', x + width, y, 0xAAAAAA);
	for (uint8_t i = 0; i < (width - 1); i++) {
		putcAt('#', x + 1 + i, y, 0x454550);
	}
}