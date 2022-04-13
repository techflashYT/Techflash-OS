#include "../util.h"
void farpokeb(uint16_t sel, void* off, uint8_t v) {
	__asm__(
        "push %%fs\r\n"
        "mov  %0, %%fs\r\n"
        "movb %2, %%fs:(%1)\r\n"
        "pop %%fs"
        : : "g"(sel), "r"(off), "r"(v)
    );
}
