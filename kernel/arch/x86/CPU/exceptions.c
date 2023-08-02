#include <kernel/panic.h>
#include <kernel/arch/x86/ISR.h>
#include <stdio.h>
// 0
static void zeroDivHandler(registers_t *regs) {panic("(#DE) Integer Division by Zero", regs);}
// 1, 3
static void debugHandler(__attribute__ ((unused)) registers_t *regs) {/* TODO: Start a kernel debugger here */printf("\r\nWould start a kernel debugger here but it's not implemented!\r\n");}
// 2
static void NMIHandler(registers_t *regs) {panic("Non-Maskable Interrupt", regs);}
// 4
static void overflowHandler(registers_t *regs) {panic("(#OF) Overflow", regs);}
// 5
static void boundRangeExceededHandler(registers_t *regs) {panic("(#BR) Bound Range Exceeded", regs);}
// 6
static void invalidOpcodeHandler(registers_t *regs) {panic("(#UD) Invalid Opcode (likely a bad jump)", regs);}
/*
	7 is "Device not available", which is used when an FPU instruction is executed but there is no FPU present.
	This is simply impossible as the bootloader that this OS uses will not run on a CPU that is that old.
*/
// FIXME: Implementing it anyways because it happened???
static void deviceNotAvailableHandler(registers_t *regs) {panic("(#NM) Device Not Available", regs);}
// 8
static void doubleFaultHandler(registers_t *regs) {panic("(#DF) Double Fault", regs);}
/*
	9 is "Coprocessor Segment Overrun", this is when the FPU would perform segment checking and it detected an overrun.
	This is no longer in effect as the modern internal FPU just raises a #GP for segment overruns.
*/
/*
	10 is "Invalid TSS", since we don't use TSS yet, this is unimplemented.
*/
// 11 (0xB)
static void segmentNotPresetHandler(registers_t *regs) {panic("(#NP) Segment Not Present", regs);}
// 12 (0xC)
static void stackSegmentFaultHandler(registers_t *regs) {panic("(#SS) Stack Segment Fault", regs);}
// 13 (0xD)
static void generalProtectionFaultHandler(registers_t *regs) {panic("(#GP) General Protection Fault", regs);}
// 14 (0xE)
static void pageFaultHandler(registers_t *regs) {panic("(#PF) Page Fault", regs);}
// 15 (0xF), 22-27 (0x16-0x1B), 31 (0x1F)
static void reservedHandler(registers_t *regs) {panic("Reserved Exception.... wtf????????", regs);}
// 16 (0x10)
static void FPUExceptionHandler(registers_t *regs) {panic("(#MF) Floating Point Exception", regs);}
// 17 (0x11)
static void alignmentCheckExceptionHandler(registers_t *regs) {panic("(#AC) Alignment Check Exception", regs);}
// 18 (0x12)
static void machineCheckExceptionHandler(registers_t *regs) {panic("(#MC) Machine Check Exception", regs);}
// 19 (0x13)
static void SSEExceptionHandler(registers_t *regs) {panic("(#XF) SSE Exception", regs);}
// 20 (0x14)
static void virtualizationExceptionHandler(registers_t *regs) {panic("(#VE) Virtualization Exception", regs);}
// 21 (0x15)
static void controlProtectionExceptionHandler(registers_t *regs) {panic("(#CP) Control Protection Exception", regs);}
// 28 (0x1C)
static void hypervisorInjectionExceptionHandler(registers_t *regs) {panic("(#HV) Hypervisor Injection Exception", regs);}
// 29 (0x1D)
static void VMMCommunicationExceptionHandler(registers_t *regs) {panic("(#VC) VMM Communication Exception", regs);}
// 30 (0x1E)
static void securityExceptionHandler(registers_t *regs) {panic("(#SX) Security Exception", regs);}


void x86_Exceptions_Init() {
	x86_ISR_RegisterHandler(0,  &zeroDivHandler);
	x86_ISR_RegisterHandler(1,  &debugHandler);
	x86_ISR_RegisterHandler(2,  &NMIHandler);
	x86_ISR_RegisterHandler(3,  &debugHandler);
	x86_ISR_RegisterHandler(4,  &overflowHandler);
	x86_ISR_RegisterHandler(5,  &boundRangeExceededHandler);
	x86_ISR_RegisterHandler(6,  &invalidOpcodeHandler);
	x86_ISR_RegisterHandler(7,  &deviceNotAvailableHandler);
	x86_ISR_RegisterHandler(8,  &doubleFaultHandler);
	x86_ISR_RegisterHandler(11, &segmentNotPresetHandler);
	x86_ISR_RegisterHandler(12, &stackSegmentFaultHandler);
	x86_ISR_RegisterHandler(13, &generalProtectionFaultHandler);
	x86_ISR_RegisterHandler(14, &pageFaultHandler);
	x86_ISR_RegisterHandler(15, &reservedHandler);
	x86_ISR_RegisterHandler(16, &FPUExceptionHandler);
	x86_ISR_RegisterHandler(17, &alignmentCheckExceptionHandler);
	x86_ISR_RegisterHandler(18, &machineCheckExceptionHandler);
	x86_ISR_RegisterHandler(19, &SSEExceptionHandler);
	x86_ISR_RegisterHandler(20, &virtualizationExceptionHandler);
	x86_ISR_RegisterHandler(21, &controlProtectionExceptionHandler);
	x86_ISR_RegisterHandler(22, &reservedHandler);
	x86_ISR_RegisterHandler(23, &reservedHandler);
	x86_ISR_RegisterHandler(24, &reservedHandler);
	x86_ISR_RegisterHandler(25, &reservedHandler);
	x86_ISR_RegisterHandler(26, &reservedHandler);
	x86_ISR_RegisterHandler(27, &reservedHandler);
	x86_ISR_RegisterHandler(28, &hypervisorInjectionExceptionHandler);
	x86_ISR_RegisterHandler(29, &VMMCommunicationExceptionHandler);
	x86_ISR_RegisterHandler(30, &securityExceptionHandler);
	x86_ISR_RegisterHandler(31, &reservedHandler);
}