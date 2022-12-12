#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <kernel/environment.h>
#include <kernel/font.h>
#include <kernel/hardware/serial.h>
#include <kernel/tty.h>
// Thanks to @quietfanatic on StackOverflow for this code, they helped make this actually optimized, check out their comment:
// https://stackoverflow.com/a/74765904/16387557
// along with my original question of how to optimize it:
// https://stackoverflow.com/questions/74765778/c-osdev-how-could-i-shift-the-contents-of-a-32-bit-framebuffer-upwards-effic
void kernTTY_scroll(const char *numLines) {
	// Convert string to integer
	uint32_t numLinesInt = atoi(numLines);

	// Figure out how many pixels we need to move in order to move 1 line
	numLinesInt *= font->height;

	// The destination of the move is just the top of the framebuffer
	uint32_t* destination = (uint32_t*)&fb;

	// Start the move from the top of the framebuffer plus number
	// of lines to scroll.
	uint32_t* source = (uint32_t*)&fb + (numLinesInt * bootboot.fb_width);

	// The total number of pixels to move is the size of the
	// framebuffer minus the amount of lines we want to scroll.
	uint32_t pixel_size = (bootboot.fb_height - numLinesInt) * bootboot.fb_width;

	// The total number of bytes is that times the size of one pixel.
	uint32_t byteSize = pixel_size * sizeof(uint32_t);

	// Do the move
	memmove(destination, source, byteSize);
}