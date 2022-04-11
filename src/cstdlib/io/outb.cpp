#include "../util.hpp"
void outb(uint16_t port, uint8_t val) {
	asm volatile(
        "outb %0, %1" : : "a"(val), "Nd"(port)
    );
}
