#include <kernel/hardware/CPU/x86Setup.h>
#include <kernel/hardware/IO.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <kernel/panic.h>
#include <kernel/custom.h>
#include <kernel/tty.h>
MODULE("X86INT");
#define PIC1		 0x20		/* IO base address for master PIC */
#define PIC2		 0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND PIC1
#define PIC1_DATA	 (PIC1+1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA	 (PIC2+1)
#define PIC_EOI		 0x20
isr_t interruptHandlers[256] = {0};
uint64_t lastInterruptNumber = 0;
uint8_t intMsgNumOffset = 13;
void sendEOI() {
	if(lastInterruptNumber >= 8) {
		outb(PIC2_COMMAND, PIC_EOI);
	}
 
	outb(PIC1_COMMAND, PIC_EOI);
}
void ISRHandler(registers_t* regs) {
	regs->intNo += 128;
	lastInterruptNumber = regs->intNo;
	if (interruptHandlers[regs->intNo] != 0) {
		isr_t handler = interruptHandlers[regs->intNo];
		handler(regs);
		return;
	}
	// panic("Unhandled Interrupt", regs);
	// log it, pray that it isn't necessary, and continue
	char buffer[87];
	sprintf(buffer, "Interrupt 0x%02lx triggered with no handler!  Ignoring it and hoping nothing bad happens.", regs->intNo);
	log(MODNAME, buffer, LOGLEVEL_WARN);
}
void registerInterruptHandler(uint8_t n, isr_t handler) {
	char str[72];
	sprintf(str, "Registering function %p to be called on interrupt 0x%02X.", handler, n);
	
	log(MODNAME, str, LOGLEVEL_VERBOSE);
	interruptHandlers[n] = handler;
}

