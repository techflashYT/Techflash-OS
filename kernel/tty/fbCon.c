#if CONFIG_FBCON == 1

#include <kernel/tty/fbCon.h>
#include <kernel/tty.h>
#include <kernel/bootloader.h>
#include <stddef.h>
#include <stdbool.h>

framebuffer_t fbCon;

MODULE("FBCON");
extern framebuffer_t LM_GetFramebuffer();

static bool esc = false;

void FBCON_Write(const char ch, const uint16_t x, const uint16_t y, const uint32_t fgColor, const uint32_t bgColor) {
	if (ch == '\x1b') {
		esc = true;
		return;
	}
	if (esc) {
		esc = !TTY_HandleEsc(ch);
		return;
	}
	if (ch == '\r') {
		TTY_CursorX = 0;
		return;
	}
	if (ch == '\n') {
		TTY_CursorY++;
		return;
	}
	if (ch == '\t') {
		TTY_CursorX += 4;
		return;
	}

	uint8_t *charData = font[(uint8_t)ch];
	uint32_t offset = ((y * 16) * fbCon.width) + (x * 8);
	for (uint_fast8_t i = 0; i != 16; i++) {
		for (uint_fast8_t j = 7; j != 0; j--) {
			uint32_t color = bgColor;
			if (charData[i] & (1 << j)) {
				color = fgColor;
				if (TTY_Bold) {
					// FIXME: for VGA yellow, we need to add 0xAA to the green for some reason
					if (color == COLOR_YELLOW) {
						color = 0xFFFF55;
						goto done;
					}
					// increase the brightness of the color by incresing all but the first byte of the uint32_t value
					for (uint_fast8_t k = 0; k != 4; k++) {
						uint8_t originalByte = ((uint8_t*)&color)[k];
						((uint8_t *)&color)[k] += 0x55;

						// Check for overflow
						if (((uint8_t *)&color)[k] < originalByte) {
							((uint8_t *)&color)[k] = 0xFF;
						}
					}
				}
			}
			done:
			fbCon.ptr[offset + (7 - j) + (i * fbCon.width)] = color;
		}
	}
	
	TTY_CursorX++;
	if (TTY_CursorX >= TTY_Width) {
		TTY_CursorX = 0;
		TTY_CursorY++;
	}
	if (TTY_CursorY > TTY_Height) {
		// TODO: Scroll TTY
	}
}

static bool firstRun = true;

static void FBCON_InitSimple() {
	if (BOOT_LoaderID == BOOT_LoaderID_LimineCompatible) {
		fbCon = LM_GetFramebuffer();
	}
	else {
		log("Unknown bootloader.  Unable to set framebuffer!  Keeping print method to default, you will probably not have video output!", LOGLEVEL_FATAL);
		fbCon = (framebuffer_t){.ptr = NULL, .width = 0, .height = 0, .mode = 0};
	}

	TTY_CursorX  = 0;
	TTY_CursorY  = 0;
	TTY_Width    = (uint16_t)(fbCon.width  / 8);
	TTY_Height   = (uint16_t)(fbCon.height / 16);
	TTY_Color    = COLOR_LGRAY;
	TTY_Bold     = false;

	if (fbCon.ptr != 0) {
		log("Setting TTY Write func", LOGLEVEL_DEBUG);
		TTY_SetWriteFunc(FBCON_Write);
		log("Done setting TTY Write func", LOGLEVEL_DEBUG);
	}
}

static void FBCON_InitReal() {
	log("Not implemented I guess", LOGLEVEL_FATAL);
}
void FBCON_Init() {
	if (firstRun) {
		FBCON_InitSimple();
		firstRun = false;
		return;
	}
	FBCON_InitReal();
	return;
}
#endif