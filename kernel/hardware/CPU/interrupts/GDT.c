#include <stdint.h>
#include <kernel/hardware/CPU/GDT.h>
#include <kernel/hardware/CPU/IDT.h>

gdtEntry_t gdtEntries[5];
gdtPtr_t   gdtPtr;

extern void GDTFlush();

static void GDTSetGate(int32_t num, uint64_t base, uint32_t limit, uint8_t access, uint8_t gran) {
	gdtEntries[num].baseLow = (base & 0xFFFF);
	gdtEntries[num].baseMiddle = (base >> 16) & 0xFF;
	gdtEntries[num].baseHigh = (base >> 24) & 0xFF;

	gdtEntries[num].limitLow = (limit & 0xFFFF);
	gdtEntries[num].granularity = (limit >> 16) & 0x0F;

	gdtEntries[num].granularity |= gran & 0xF0;
	gdtEntries[num].access = access;
}

void GDTInit() {
	gdtPtr.limit = (sizeof(gdtEntry_t) * 5) - 1;
	gdtPtr.base  = (uint64_t)&gdtEntries;
	GDTSetGate(0, 0, 0, 0, 0);                // Null segment
	GDTSetGate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
	GDTSetGate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
	GDTSetGate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
	GDTSetGate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment

	GDTFlush((uint64_t)&gdtPtr);
}
