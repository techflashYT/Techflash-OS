#include "../../../../shared.h"
#include "../../../../cstdlib/defs.h"
struct idtEntry {
	unsigned short base_low;
	unsigned short sel;
	unsigned char zero;
	unsigned char flags;
	unsigned short base_high;
} __attribute__((packed));

/*
 * IDT pointer
 */
struct idtPtr {
	unsigned short limit;
	uintptr_t base;
} __attribute__((packed));

struct idtEntry idt[256];
struct idtPtr idtp;

extern void idtLoad();

void idtSetGate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags) {
	idt[num].base_low   =	(base & 0xFFFF);
	idt[num].base_high  =	(base >> 16) & 0xFFFF;
	idt[num].sel        =   sel;
	idt[num].zero 		=   0;
	idt[num].flags 		=   flags | 0x60;
}
void idtInstall() {
	idtp.limit = (sizeof(struct idtEntry) * 256) - 1;
	idtp.base = (uintptr_t)&idt;
	memset(&idt, 0, sizeof(struct idtEntry) * 256);

	idt_load();
}
