#include <stdint.h>
#include <kernel/hardware/CPU/IDT.h>
idtEntry_t idtEntries[256];
// idtPtr_t   idtPtr;
extern void IDTFlush();
void IDTInit() {
	for (uint64_t i = 0; i < 256; i++) {
		idtEntries[i].reserved   = 0;
		idtEntries[i].baseLow    = (uint16_t)(((uint64_t)&ISR0 & 0x000000000000FFFF));
		idtEntries[i].baseMiddle = (uint16_t)(((uint64_t)&ISR0 & 0x00000000FFFF0000) >> 16);
		idtEntries[i].baseMiddle = (uint32_t)(((uint64_t)&ISR0 & 0xFFFFFFFF00000000) >> 32);
		idtEntries[i].always0    = 0;
		idtEntries[i].sel        = 0x08;
		idtEntries[i].flags      = 0x8E;
	}
	IDTFlush();
}