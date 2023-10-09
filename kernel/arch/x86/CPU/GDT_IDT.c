#include <stdio.h>
#include <kernel/arch/x86/IO.h>
#include <kernel/arch/x86/GDT_IDT.h>

MODULE("X86INT");

static x86_GDT_Ptr64_t   gdtPtr;
static x86_GDT_Entry64_t gdtEntries[5];
static void x86_GDT_SetGate(int32_t num, uint64_t base, uint32_t limit, uint8_t access, uint8_t gran) {
	(void)num;
	gdtEntries[num].baseLow = (base & 0xFFFF);
	gdtEntries[num].baseMiddle = (base >> 16) & 0xFF;
	gdtEntries[num].baseHigh = (base >> 24) & 0xFF;

	gdtEntries[num].limitLow = (limit & 0xFFFF);
	gdtEntries[num].granularity = (limit >> 16) & 0x0F;

	gdtEntries[num].granularity |= gran & 0xF0;
	gdtEntries[num].access = access;
}

extern void x86_GDT_Flush(x86_GDT_Ptr64_t *ptr);
void x86_GDT_Init() {
	log("GDT Initializing", LOGLEVEL_INFO);
	gdtPtr.limit = (sizeof(x86_GDT_Entry64_t) * 5) - 1;
	gdtPtr.base  = (uint64_t)&gdtEntries;
	x86_GDT_SetGate(0, 0, 0, 0, 0);                // Null segment
	x86_GDT_SetGate(1, 0, 0xFFFFFFFF, 0x9A, 0xAF); // Code segment
	x86_GDT_SetGate(2, 0, 0xFFFFFFFF, 0x92, 0xAF); // Data segment
	x86_GDT_SetGate(3, 0, 0xFFFFFFFF, 0xFA, 0xAF); // User mode code segment
	x86_GDT_SetGate(4, 0, 0xFFFFFFFF, 0xF2, 0xAF); // User mode data segment

	x86_GDT_Flush(&gdtPtr);
	log("GDT Loaded!", LOGLEVEL_INFO);
}



static x86_IDT_Entry64_t x86_IDT_Entries[256];
static x86_IDT_Descriptor_t x86_IDT_Descriptor = {
	4095,
	x86_IDT_Entries
};

extern const uint64_t x86_ISR_StubArray[256];
static void x86_IDT_SetGate(uint8_t num, uint64_t base, uint16_t sel, uint8_t flags) {
	(void)x86_IDT_Descriptor;

	x86_IDT_Entries[num].baseLow = (base & 0xFFFF);
	x86_IDT_Entries[num].baseMiddle = (base >> 16) & 0xFFFF;
	x86_IDT_Entries[num].baseHigh = (uint32_t)((base >> 32) & 0xFFFFFFFF);

	x86_IDT_Entries[num].sel = sel;
	x86_IDT_Entries[num].always0 = 0;

	// Uncomment the OR below for user mode sets the instruction privilege level to ring 3.
	x86_IDT_Entries[num].flags = flags /* | 0x60 */;
	x86_IDT_Entries->reserved = 0;
}

void x86_IDT_Init() {
	log("IDT Initializing", LOGLEVEL_INFO);
	// Remap IRQ table for PIC.
	outb(0x20, 0x11);
	outb(0xA0, 0x11);
	outb(0x21, 0x20);
	outb(0xA1, 0x28);
	outb(0x21, 0x04);
	outb(0xA1, 0x02);
	outb(0x21, 0x01);
	outb(0xA1, 0x01);
	outb(0x21, 0x0);
	outb(0xA1, 0x0);
	for (uint8_t i = 0; i != 0xFF; i++) {
		x86_IDT_SetGate(i, (uint64_t)&x86_ISR_StubArray[i], 0x08, 0x8E);
	}


	uint64_t junk;
	asm("leaq x86_IDT_Descriptor(%%rip), %0; lidt (%0)" : "=a"(junk));
	log("IDT Loaded!", LOGLEVEL_INFO);
}