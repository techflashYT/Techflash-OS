#include <stdio.h>
#include <kernel/arch/x86/IO.h>
MODULE("X86INT");
void x86_GDT_Init() {
	log(MODNAME, "Not messing with GDT since Limine's probably just works, and is noted in the spec", LOGLEVEL_WARN);
}

typedef struct {
	uint16_t a;
	void *b;
} __attribute__((packed)) x86_IDT_Descriptor_t;


typedef struct {
	uint16_t baseLow;             // Bit 0-15 of the address to jump to when this interrupt fires.
	uint16_t sel;                 // Kernel segment selector.
	uint8_t  always0;             // This must always be zero.
	uint8_t  flags;               // More flags. See documentation.
	uint16_t baseMiddle;          // Bits 16-31 of the address to jump to.
	uint32_t baseHigh;			  // Bits 32-63 of the address to jump to.
	uint32_t reserved;			  // Reserved bits.
} __attribute__((packed)) x86_IDT_Entry64_t;

static x86_IDT_Entry64_t x86_IDT_Entries[256];
x86_IDT_Descriptor_t x86_IDT_Descriptor = {
	4095,
	x86_IDT_Entries
};

extern const uint64_t x86_ISR_StubArray[256];
static void x86_IDT_SetGate(uint8_t num, uint64_t base, uint16_t sel, uint8_t flags) {
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
	log(MODNAME, "IDT Initializing", LOGLEVEL_INFO);
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
		x86_IDT_SetGate(i, (uint64_t)&x86_ISR_StubArray[i], 0x28, 0x8E);
	}


	asm("lidt (x86_IDT_Descriptor)");
	log(MODNAME, "IDT Loaded!", LOGLEVEL_INFO);
}