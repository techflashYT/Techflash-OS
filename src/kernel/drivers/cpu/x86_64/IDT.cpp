#include <cstdint>
#include <cstdlib>
#include "../../../inc/util.hpp"
void idtLoad();
struct idtEntry {
    uint_fast16_t baseLow;
	uint_fast16_t sel;
	uint_fast8_t zero;
	uint_fast8_t flags;
	uint_fast16_t baseHigh;
} __attribute__((packed));


struct idtPtr {
	unsigned short limit;
	uintptr_t base;
} __attribute__((packed));

struct idtEntry idt[256];
struct idtPtr idtp;


void idtSetGate(uint_fast8_t num, uint_fast64_t base, uint_fast16_t sel, uint_fast8_t flags) {
    idt[num].baseLow = (base & 0xFFFF);
    idt[num].baseHigh = (base >> 16) & 0xFFFF;
    idt[num].sel = sel;
    idt[num].zero = 0;
    idt[num].flags = flags | 0x60;
}

void idtInstall() {
    idtp.limit = (sizeof(struct idtEntry) * 256) - 1;
    idtp.base = (uintptr_t)&idt;
    memset(&idt, 0, sizeof(struct idtEntry) * 256);

    idtLoad();
}
