#include <kernel/tty.h>
#include <kernel/environment.h>
#include <kernel/newFont.h>
#include <string.h>
#include <assert.h>

#include <assert.h>

static void TTY_ScrollBasic(uint_fast16_t numLines) {
	int width, height;
	UNPACK_WIDTH_HEIGHT(font.header.widthHeight, width, height);
	(void)width;

	uint32_t* fbAddress = (uint32_t*)fb->address;
	uint_fast64_t fbWidth = fb->width;
	uint_fast64_t fbHeight = fb->height;
	uint_fast64_t fbPitch = fb->pitch / sizeof(uint32_t);

	// Check if the number of lines to scroll is within the valid range
	assert(numLines < fbHeight);

	// Check if the framebuffer address is not NULL
	assert(fbAddress != NULL);

	// Check if the framebuffer width and height are greater than 0
	assert(fbWidth > 0);
	assert(fbHeight > 0);

	if (numLines >= fbHeight) {
		numLines = fbHeight - 1;
	}
	uint_fast64_t shiftNum = numLines * height;

	// Check if the scrolling operation stays within the framebuffer boundaries
	assert(shiftNum <= fbHeight);

	// Check if the font height is greater than 0
	assert(height > 0);

	// Check if the framebuffer address is properly aligned
	assert(((uintptr_t)fbAddress) % sizeof(uint32_t) == 0);

	// Shift the lines up
	for (uint_fast64_t y = shiftNum; y < fbHeight; y++) {
		for (uint_fast64_t x = 0; x < fbWidth; x++) {
			fbAddress[(y - shiftNum) * fbPitch + x] = fbAddress[y * fbPitch + x];
		}
	}

	// Clear the new lines at the bottom
	for (uint_fast64_t y = fbHeight - shiftNum; y < fbHeight; y++) {
		for (uint_fast64_t x = 0; x < fbWidth; x++) {
			fbAddress[y * fbPitch + x] = TTY_TextBackground;
		}
	}

	TTY_CursorY -= numLines;
}





void TTY_Scroll(uint_fast16_t numLines) {
	if (!TTY_Ready) {
		TTY_ScrollBasic(numLines);
	}
	// TODO: Shift full char buffer up
}
