#include <stdint.h>
#include <string.h>
#include <kernel/hardware/IDT.h>

idtEntry_t idtEntries[256];
idtPtr_t   idtPtr;
extern void IDTFlush(uint32_t unkown);

static void IDTSetGate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
	idtEntries[num].baseLow = (base & 0xFFFF);
	idtEntries[num].baseHigh = (base >> 16) & 0xFFFF;

	idtEntries[num].sel = sel;
	idtEntries[num].always0 = 0;

	// Uncoment the OR below for user mode sets the instruction privilege level to ring 3.
	idtEntries[num].flags = flags /* | 0x60 */;
}

void IDTInit() {
	idtPtr.limit = (sizeof(idtEntry_t) * 256) - 1;
	idtPtr.base = (uint64_t)&idtEntries;

	memset(&idtEntries, 0, sizeof(idtEntry_t) * 256);

	IDTSetGate(0, (uint64_t)ISR0, 0x08, 0x8E);
	IDTSetGate(1, (uint64_t)ISR1, 0x08, 0x8E);
	IDTSetGate(2, (uint64_t)ISR2, 0x08, 0x8E);
	IDTSetGate(3, (uint64_t)ISR3, 0x08, 0x8E);
	IDTSetGate(4, (uint64_t)ISR4, 0x08, 0x8E);
	IDTSetGate(5, (uint64_t)ISR5, 0x08, 0x8E);
	IDTSetGate(6, (uint64_t)ISR6, 0x08, 0x8E);
	IDTSetGate(7, (uint64_t)ISR7, 0x08, 0x8E);
	IDTSetGate(8, (uint64_t)ISR8, 0x08, 0x8E);
	IDTSetGate(9, (uint64_t)ISR9, 0x08, 0x8E);
	IDTSetGate(10, (uint64_t)ISR10, 0x08, 0x8E);
	IDTSetGate(11, (uint64_t)ISR11, 0x08, 0x8E);
	IDTSetGate(12, (uint64_t)ISR12, 0x08, 0x8E);
	IDTSetGate(13, (uint64_t)ISR13, 0x08, 0x8E);
	IDTSetGate(14, (uint64_t)ISR14, 0x08, 0x8E);
	IDTSetGate(15, (uint64_t)ISR15, 0x08, 0x8E);
	IDTSetGate(16, (uint64_t)ISR16, 0x08, 0x8E);
	IDTSetGate(17, (uint64_t)ISR17, 0x08, 0x8E);
	IDTSetGate(18, (uint64_t)ISR18, 0x08, 0x8E);
	IDTSetGate(19, (uint64_t)ISR19, 0x08, 0x8E);
	IDTSetGate(20, (uint64_t)ISR20, 0x08, 0x8E);
	IDTSetGate(21, (uint64_t)ISR21, 0x08, 0x8E);
	IDTSetGate(22, (uint64_t)ISR22, 0x08, 0x8E);
	IDTSetGate(23, (uint64_t)ISR23, 0x08, 0x8E);
	IDTSetGate(24, (uint64_t)ISR24, 0x08, 0x8E);
	IDTSetGate(25, (uint64_t)ISR25, 0x08, 0x8E);
	IDTSetGate(26, (uint64_t)ISR26, 0x08, 0x8E);
	IDTSetGate(27, (uint64_t)ISR27, 0x08, 0x8E);
	IDTSetGate(28, (uint64_t)ISR28, 0x08, 0x8E);
	IDTSetGate(29, (uint64_t)ISR29, 0x08, 0x8E);
	IDTSetGate(30, (uint64_t)ISR30, 0x08, 0x8E);
	IDTSetGate(31, (uint64_t)ISR31, 0x08, 0x8E);

	IDTFlush((uint64_t)&idtPtr);
}