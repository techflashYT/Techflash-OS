#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <kernel/environment.h>
#include <kernel/font.h>
#include <kernel/hardware/serial.h>
#include <kernel/tty.h>
#include <kernel/panic.h>
// Thanks to @quietfanatic on StackOverflow for this code, they helped make this actually optimized, check out their comment:
// https://stackoverflow.com/a/74765904/16387557
// along with my original question of how to optimize it:
// https://stackoverflow.com/questions/74765778/c-osdev-how-could-i-shift-the-contents-of-a-32-bit-framebuffer-upwards-effic
void TTY_Scroll(const char *numLines) {
	// Convert string to integer
	uint_fast16_t numLinesInt = atoi(numLines);

	// Test if we'll be scrolling to outside of the framebuffer
	int_fast16_t testForBounds = TTY_CursorY;
	testForBounds -= numLinesInt;
	// Would this place it before the framebuffer?
	if (testForBounds < 0) {
		// Yes, panic
		DUMPREGS;
		panic("TTY: Attempted to scroll to an invalid line!", regs);
	}
	// Figure out how many pixels we need to move in order to move 1 line
	uint_fast32_t numPixels = numLinesInt * font->height;

	// The destination of the move is just the top of the framebuffer
	uint32_t* destination = (uint32_t*)fb->address;

	// Start the move from the top of the framebuffer plus number
	// of lines to scroll.
	uint32_t* source = (uint32_t*)fb->address + (numPixels * fb->width);

	// The total number of pixels to move is the size of the
	// framebuffer minus the amount of lines we want to scroll.
	uint_fast32_t pixelSize = (fb->height - numPixels) * fb->width;

	// The total number of bytes is that times the size of one pixel.
	uint_fast32_t byteSize = pixelSize * sizeof(uint32_t);

	// Save the original value for whether the cursor should blink
	bool blinkingCursorOrig = TTY_BlinkingCursor;

	// Disable the blinking cursor, it could cause issues.
	TTY_BlinkingCursor = false;

	// Do the move
	memmove(destination, source, byteSize);

	
	// Set the cursor to the right position
	TTY_CursorY -= numLinesInt;

	// Set the blinking cursor to the previous value.
	TTY_BlinkingCursor = blinkingCursorOrig;
	return;
}
