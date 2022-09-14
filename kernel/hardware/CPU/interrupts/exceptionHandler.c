#include <kernel/hardware/CPU/ISR.h>
#include <kernel/panic.h>
void doubleFaultHandler(registers_t *regs) {
	panic("Double fault detected", *regs);
}
void initExceptions() {
	registerInterruptHandler(8, &doubleFaultHandler);
}