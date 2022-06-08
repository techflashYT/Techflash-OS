#ifndef ARCH_I386_IDT_H
#define ARCH_I386_IDT_H

#include <stdint.h>
#define KERNEL_CS 0x08
typedef struct {
    uint16_t lowOffset;
    uint16_t sel;
    uint8_t always0;
    /* First byte
     * Bit 7: "Interrupt is present"
     * Bits 6-5: Privilege level of caller (0=kernel..3=user)
     * Bit 4: Set to 0 for interrupt gates
     * Bits 3-0: bits 1110 = decimal 14 = "32 bit interrupt gate" */
    uint8_t flags; 
    uint16_t highOffset;
} __attribute__((packed)) idtGate_t;

typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idtRegister_t;

#define IDT_ENTRIES 256


void setIdtGate(int n, uint32_t handler);
void setIdt();

#endif
