#include <kernel/tty/fbCon.h>
#include <kernel/tty.h>
#include <kernel/bootloader.h>
#include <stddef.h>

framebuffer_t fbCon;

MODULE("FBCON");
extern framebuffer_t LM_GetFramebuffer();

static bool esc = false;

void FBCON_Write(const char ch, const uint16_t x, const uint16_t y, const uint32_t fgColor, const uint32_t bgColor) {
	if (ch == '\r') {
		TTY_CursorX = 0;
		return;
	}
	if (ch == '\n') {
		TTY_CursorY++;
		return;
	}
	if (ch == '\x1b') {
		esc = true;
	}
	if (esc) {
		esc = !TTY_HandleEsc(ch);
		return;
	}
	(void)ch;
	(void)x;
	(void)y;
	(void)fgColor;
	(void)bgColor;
	uint8_t *charData = font[(uint8_t)ch];
	uint32_t offset = ((y * 16) * fbCon.width) + (x * 8);
	for (uint_fast8_t i = 0; i != 16; i++) {
		for (uint_fast8_t j = 0; j != 8; j++) {
			uint32_t color = bgColor;
			if (charData[i] & (1 << j)) {
				color = fgColor;
			}
			fbCon.ptr[offset + j + (i * fbCon.width)] = color;
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

framebuffer_t FBCON_Init() {
	framebuffer_t ret;
	if (BOOT_LoaderID == BOOT_LoaderID_LimineCompatible) {
		ret = LM_GetFramebuffer();
	}
	else {
		log(MODNAME, "Unknown bootloader.  Unable to set framebuffer!  Keeping print method to default, you will probably not have video output!", LOGLEVEL_FATAL);
		ret = (framebuffer_t){.ptr = NULL, .width = 0, .height = 0, .mode = 0};
	}
	fbCon.width  = ret.width;
	fbCon.height = ret.height;
	fbCon.mode   = ret.mode;
	fbCon.ptr    = ret.ptr;


	TTY_CursorX  = 0;
	TTY_CursorY  = 0;
	TTY_Width    = (uint16_t)(ret.width  / 8);
	TTY_Height   = (uint16_t)(ret.height / 8);
	TTY_Color    = COLOR_LGRAY;

	if (ret.ptr != 0) {
		log(MODNAME, "Setting TTY Write func", LOGLEVEL_DEBUG);
		TTY_SetWriteFunc(FBCON_Write);
		log(MODNAME, "Done setting TTY Write func", LOGLEVEL_DEBUG);
	}

	return ret;
}