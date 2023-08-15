#include <kernel/tty/fbCon.h>
#include <kernel/tty.h>
#include <kernel/bootloader.h>
#include <stddef.h>

framebuffer_t fbCon;

MODULE("FBCON");
extern framebuffer_t LM_GetFramebuffer();

void FBCON_Write(const char ch, const uint16_t x, const uint16_t y, const uint32_t fgColor, const uint32_t bgColor) {
	(void)ch;
	(void)x;
	(void)y;
	(void)fgColor;
	(void)bgColor;
	uint8_t charData[16] = {
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00011000,
		0b01100110,
		0b01100110,
		0b01100110,
		0b01100110,
		0b01111110,
		0b01111110,
		0b01100110,
		0b01100110,
		0b01100110,
		0b00000000,
	};
	uint32_t offset = ((y * 16) * fbCon.width) + (x * 8);
	for (uint_fast8_t i = 0; i != 16; i++) {
		for (uint_fast8_t j = 0; j != 8; j++) {
			uint32_t color = bgColor;
			if (charData[i] & (1 << j)) {
				color = fgColor;
			}
			fbCon.ptr[offset + j + (i * fbCon.height)] = color;
		}
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

	if (ret.ptr != 0) {
		log(MODNAME, "Setting TTY Write func", LOGLEVEL_DEBUG);
		TTY_SetWriteFunc(FBCON_Write);
		log(MODNAME, "Done setting TTY Write func", LOGLEVEL_DEBUG);
	}

	return ret;
}