#include <kernel/hardware/CPU/x86Setup.h>
#include <kernel/hardware/IO.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <kernel/panic.h>
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
char *badIntMsg = "Interrupt '0xAA' triggered with no handler!  Ignoring it and hoping nothing bad happens.";
uint8_t intMsgNumOffset = 13;
void sendEOI() {
	if(lastInterruptNumber >= 8) {
		outb(PIC2_COMMAND, PIC_EOI);
	}
 
	outb(PIC1_COMMAND, PIC_EOI);
}
extern void padTo(char *src, uint8_t padding);
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
	char *tempBuffer = malloc(8);
	utoa(regs->intNo, tempBuffer, 16);
	padTo(tempBuffer, 2);
	badIntMsg[intMsgNumOffset] = tempBuffer[0];
	badIntMsg[intMsgNumOffset + 1] = tempBuffer[1];
	log(MODNAME, badIntMsg, LOGLEVEL_WARN);
	free(tempBuffer);
}
void registerInterruptHandler(uint8_t n, isr_t handler) {
	char *str = "Registering function 0xAAAAAAAAAAAAAAAA to be called on interrupt 0xAA.";
	uint8_t offset1 = 23;
	uint8_t offset2 = 68;

	char *buffer = malloc(17);
	utoa((uint64_t)handler, buffer, 16);
	memcpy(str + offset1, buffer, 16);
	
	
	utoa((uint64_t)n, buffer, 16);
	padTo(buffer, 2);
	memcpy(str + offset2, buffer, 2);

	log(MODNAME, str, LOGLEVEL_VERBOSE);
	interruptHandlers[n] = handler;
}

