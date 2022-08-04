#include <kernel/hardware/CPU/regs.h>
#include <kernel/hardware/CPU/ISR.h>
#include <kernel/hardware/IO.h>
extern isr_t interruptHandlers[256];
void IRQHandler(registers_t regs) {
	// Send End of Interrupt signal to the PIC.
	// If interrupt is from slave
	if (regs.intNo >= 40) {
		// Send reset signal to slave.
		outb(0xA0, 0x20);
	}
	// Send reset signal to master.
	outb(0x20, 0x20);

	if (interruptHandlers[regs.intNo] != 0) {
		isr_t handler = interruptHandlers[regs.intNo];
		handler(regs);
	}
}