#include "../util.h"
void outb(uint16_t port, uint8_t val) {
	__asm__ volatile(
        "outb %0, %1" : : "a"(val), "Nd"(port)
    );
}
