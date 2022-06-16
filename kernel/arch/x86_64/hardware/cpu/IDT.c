#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

#include <kernel/arch/i386/IDT.h>
#include <kernel/arch/i386/io.h>
typedef struct {
	uint16_t offsetLo;
	uint16_t selector;
	uint8_t ist;
	uint8_t typesAttributes;
	uint16_t offsetMd;
	uint16_t offsetHi;
	uint32_t zero;
} IDT64;


extern IDT64 _idt[256];
extern uint64_t isr1;
extern void loadIDT();


void IDTinit() {
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wtype-limits"
	#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
	for (uint8_t i = 0; i < 256; i++) {
		_idt[i].zero = 0;
		_idt[i].offsetLo = (uint16_t)(((uint64_t)&isr1 & 0x000000000000FFFF));
		_idt[i].offsetMd = (uint16_t)(((uint64_t)&isr1 & 0x00000000FFFF0000) >> 16);
		_idt[i].offsetHi = (uint16_t)(((uint64_t)&isr1 & 0xFFFFFFFF00000000) >> 32);
		_idt[i].ist = 0;
		_idt[i].selector = 0x08;
		_idt[i].typesAttributes = 0x8E;
	}
	#pragma GCC diagnostic pop
	outb(0x21, 0xFD);
	outb(0xA1, 0xFF);

	loadIDT();
}

void isr1Handler() { // Keyboard
	putchar(inb(0x60));
	outb(0x20, 0x20);
	outb(0xA0, 0x20);
}
