#include <kernel/hardware/IO.h>
#include <stdint.h>
#include <stdbool.h>
#define PIC1_DATA 0x21
#define PIC2_DATA 0xA1

void IRQSetMask(uint8_t IRQline, bool status) {
	uint16_t port;
	uint8_t value;
	if (!status) {
		if (IRQline < 8) {
			port = PIC1_DATA;
		}
		else {
			port = PIC2_DATA;
			IRQline -= 8;
		}
		value = inb(port) | (1 << IRQline);
		outb(port, value);
	}
	else {
		if (IRQline < 8) {
			port = PIC1_DATA;
		}
		else {
			port = PIC2_DATA;
			IRQline -= 8;
		}
		value = inb(port) & ~(1 << IRQline);
		outb(port, value);
	}
}