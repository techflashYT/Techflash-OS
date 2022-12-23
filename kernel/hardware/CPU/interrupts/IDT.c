#include <stdint.h>
#include <string.h>
#include <kernel/hardware/CPU/IDT.h>
#include <kernel/hardware/CPU/IRQ.h>
#include <kernel/hardware/IO.h>

idtEntry_t idtEntries[256];
extern void IDTFlush();
extern const uint64_t ISRStubArray[256];
static void IDTSetGate(uint8_t num, uint64_t base, uint16_t sel, uint8_t flags) {
	idtEntries[num].baseLow = (base & 0xFFFF);
	idtEntries[num].baseMiddle = (base >> 16) & 0xFFFF;
	idtEntries[num].baseHigh = (base >> 32) & 0xFFFFFFFF;

	idtEntries[num].sel = sel;
	idtEntries[num].always0 = 0;

	// Uncoment the OR below for user mode sets the instruction privilege level to ring 3.
	idtEntries[num].flags = flags /* | 0x60 */;
	idtEntries->reserved = 0;
}

void IDTInit() {
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
	for (uint8_t i = 0; i < 255; i++) {
		IDTSetGate(i, (uint64_t)&ISRStubArray[i], 0x08, 0x8E);
	}
	IDTSetGate(0xFF, (uint64_t)&ISRStubArray[256], 0x08, 0x8E);


	IDTFlush();
}