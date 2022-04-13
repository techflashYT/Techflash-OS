#include "../util.h"
uint32_t farpeekl(uint16_t sel, void* off) {
	uint32_t ret;
	__asm__(
        "push %%fs\r\n"
        "mov  %1, %%fs\r\n"
        "mov  %%fs:(%2), %0\r\n"
        "pop  %%fs\r\n"
        : "=r"(ret) : "g"(sel), "r"(off)
    );
	return ret;
}
