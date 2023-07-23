#include <kernel/tty.h>
#include <kernel/font.h>
#include <kernel/environment.h>
bool nextCharIsEsc = false;
#pragma GCC optimize "O0"

void FB_DrawChar(const char ch, const uint_fast16_t x, const uint_fast16_t y) {
	(void)x;
	(void)y;
	(void)ch;
}