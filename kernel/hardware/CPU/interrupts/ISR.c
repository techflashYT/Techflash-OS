#include <kernel/hardware/CPU/regs.h>
#include <kernel/hardware/CPU/ISR.h>
#include <kernel/hardware/IO.h>
#include <stdio.h>
isr_t interruptHandlers[256];
void ISRHandler(registers_t regs) {
	printf("Interrupt: %d\r\n", regs.intNo);
}
void registerInterruptHandler(uint8_t n, isr_t handler) {
	interruptHandlers[n] = handler;
}

void ISR1() {
	printf("ISR1 occurred!  Char Read: %x\r\n", inb(0x60));
	outb(0x20, 0x20);
	outb(0xA0, 0x20);
}