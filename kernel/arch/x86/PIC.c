#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <kernel/tty.h>
#include <kernel/arch/x86/ISR.h>
#include <kernel/arch/x86/IO.h>
MODULE("X86PIC");
#define PIC1		 0x20		/* IO base address for master PIC */
#define PIC2		 0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND PIC1
#define PIC1_DATA	 (PIC1+1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA	 (PIC2+1)
#define PIC_EOI		 0x20
static isr_t interruptHandlers[256] = {0};
static uint64_t lastInterruptNumber = 0;
void x86_PIC_SendEOI() {
	if(lastInterruptNumber >= 8) {
		outb(PIC2_COMMAND, PIC_EOI);
	}
 
	outb(PIC1_COMMAND, PIC_EOI);
}
// TODO: Move this crap out of PIC.c
void x86_ISR_Handler(registers_t *regs) {
	char buffer[87];

	// while(true) {}
	regs->intNo += 128;
	lastInterruptNumber = regs->intNo;
	if (interruptHandlers[regs->intNo] != 0) {
		isr_t handler = interruptHandlers[regs->intNo];
		handler(regs);
		return;
	}
	// panic("Unhandled Interrupt", regs);
	// log it, pray that it isn't necessary, and continue
	sprintf(buffer, "Interrupt 0x%02lx triggered with no handler!  Ignoring it and hoping nothing bad happens.", regs->intNo);
	log(MODNAME, buffer, LOGLEVEL_WARN);
}
void x86_ISR_RegisterHandler(uint8_t n, isr_t handler) {
	char str[72];
	sprintf(str, "Registering function %p to be called on interrupt 0x%02X.", handler, n);
	
	log(MODNAME, str, LOGLEVEL_VERBOSE);
	interruptHandlers[n] = handler;
}

