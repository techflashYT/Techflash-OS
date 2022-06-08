#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

#include <kernel/arch/i386/IDT.h>
idtGate_t idt[IDT_ENTRIES];
idtRegister_t idtReg;
#define low_16(address) (uint16_t)((address) & 0xFFFF)
#define high_16(address) (uint16_t)(((address) >> 16) & 0xFFFF)

void setIdtGate(int n, uint32_t handler) {
    idt[n].lowOffset = low_16(handler);
    idt[n].sel = KERNEL_CS;
    idt[n].always0 = 0;
    idt[n].flags = 0x8E; 
    idt[n].highOffset = high_16(handler);
}

void setIdt() {
    idtReg.base = (uint32_t) &idt;
    idtReg.limit = 262;

    __asm__ ("lidtl %0" : : "m" (idtReg));
}
