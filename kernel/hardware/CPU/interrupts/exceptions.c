#include <kernel/panic.h>
#include <stdio.h>
// 0
void zeroDivHandler(registers_t *regs) {
	panic("(#DE) Integer Division by Zero", regs);
}
// 1, 3
void debugHandler(__attribute__ ((unused)) registers_t *regs) {
	// TODO: Start a kernel debugger here
	printf("\r\nWould start a kernel debugger here but it's not implemented!\r\n");
}
// 2
void NMIHandler(registers_t *regs) {
	panic("Non-Maskable Interrupt", regs);
}
// 4
void overflowHandler(registers_t *regs) {
	panic("(#OF) Overflow", regs);
}
// 5
void boundRangeExceededHandler(registers_t *regs) {
	panic("(#BR) Bound Range Exceeded", regs);
}
// 6
void invalidOpcodeHandler(registers_t *regs) {
	panic("(#UD) Invalid Opcode (likely a bad jump)", regs);
}
/*
	7 is "Device not available", which is used when an FPU instruction is executed but there is no FPU present.
	This is simply impossible as the bootloader that this OS uses will not run on a CPU that is that old.
*/
// 8
void doubleFaultHandler(registers_t *regs) {
	panic("(#DF) Double Fault", regs);
}
/*
	9 (0xA) is "Coprocessor Segment Overrun", this is when the FPU would perform segment checking and it detected an overrun.
	This is no longer in effect as the modern internal FPU just raises a #GP for segment overruns.
*/
// 11 (0xB)
void segmentNotPresetHandler(registers_t *regs) {
	panic("(#NP) Segment Not Present", regs);
}
// 12 (0xC)
void stackSegmentFaultHandler(registers_t *regs) {
	panic("(#SS) Stack Segment Fault", regs);
}
// 13 (0xD)
void generalProtectionFaultHandler(registers_t *regs) {
	panic("(#GP) General Protection Fault", regs);
}
// 14 (0xE)
void pageFaultHandler(registers_t *regs) {
	panic("(#PF) Page Fault", regs);
}
// 15 (0xF), 22-27 (0x16-0x1B), 31 (0x1F)
void reservedHandler(registers_t *regs) {
	panic("Reserved Exception.... what????????", regs);
}
// 16 (0x10)
void FPUExceptionHandler(registers_t *regs) {
	panic("(#MF) Floating Point Exception", regs);
}
// 17 (0x11)
void alignmentCheckExceptionHandler(registers_t *regs) {
	panic("(#AC) Alignment Check Exception", regs);
}
// 18 (0x12)
void machineCheckExceptionHandler(registers_t *regs) {
	panic("(#MC) Machine Check Exception", regs);
}
// 19 (0x13)
void SSEExceptionHandler(registers_t *regs) {
	panic("(#XF) SSE Exception", regs);
}
// 20 (0x14)
void virtualizationExceptionHandler(registers_t *regs) {
	panic("(#VE) Virtualization Exception", regs);
}
// 21 (0x15)
void controlProtectionExceptionHandler(registers_t *regs) {
	panic("(#CP) Control Protection Exception", regs);
}
// 28 (0x1C)
void hypervisorInjectionExceptionHandler(registers_t *regs) {
	panic("(#HV) Hypervisor Injects Exception", regs);
}
// 29 (0x1D)
void VMMCommunicationExceptionHandler(registers_t *regs) {
	panic("(#VC) VMM Communication Exception", regs);
}
// 30 (0xFf)
void securityExceptionHandler(registers_t *regs) {
	panic("(#SX) Security Exception", regs);
}

void randomGoofyInterruptsHandler(registers_t *regs) {
	// use this to handle random stupid interrupts that we don't want to deal with.
	(void)regs;
	return;
}