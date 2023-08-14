#include <kernel/tty/fbCon.h>
#include <kernel/tty.h>
#include <kernel/bootloader.h>
#include <stddef.h>

framebuffer_t fbCon;

static bool first = true;

MODULE("FBCON");
extern framebuffer_t LM_GetFramebuffer();

void FBCON_Write(const char ch, const uint16_t x, const uint16_t y, const uint32_t fgColor, const uint32_t bgColor) {
	(void)ch;
	(void)x;
	(void)y;
	(void)fgColor;
	(void)bgColor;
	if (first) {
		first=false;
		fbCon.ptr[0] = 0xFFFFFFFF;
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

	if (ret.ptr != 0) {
		log(MODNAME, "Setting TTY Write func", LOGLEVEL_DEBUG);
		TTY_SetWriteFunc(FBCON_Write);
		log(MODNAME, "Done setting TTY Write func", LOGLEVEL_DEBUG);
	}

	fbCon = ret;
	return ret;
}