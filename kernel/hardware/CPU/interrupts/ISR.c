#include <kernel/hardware/regs.h>
#include <stdio.h>
void ISRHandler(registers_t regs) {
	printf("Interrupt: %d\r\n", regs.intNo);
}