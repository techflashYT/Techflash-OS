#include <kernel/panic.h>
#include <stdio.h>
extern void zeroDivHandler(registers_t *regs);
extern void debugHandler(registers_t *regs);
extern void NMIHandler(registers_t *regs);
extern void overflowHandler(registers_t *regs);
extern void boundRangeExceededHandler(registers_t *regs);
extern void invalidOpcodeHandler(registers_t *regs);
extern void doubleFaultHandler(registers_t *regs);
extern void segmentNotPresetHandler(registers_t *regs);
extern void stackSegmentFaultHandler(registers_t *regs);
extern void generalProtectionFaultHandler(registers_t *regs);
extern void pageFaultHandler(registers_t *regs);
extern void reservedHandler(registers_t *regs);
extern void FPUExceptionHandler(registers_t *regs);
extern void alignmentCheckExceptionHandler(registers_t *regs);
extern void machineCheckExceptionHandler(registers_t *regs);
extern void SSEExceptionHandler(registers_t *regs);
extern void virtualizationExceptionHandler(registers_t *regs);
extern void controlProtectionExceptionHandler(registers_t *regs);
extern void hypervisorInjectionExceptionHandler(registers_t *regs);
extern void VMMCommunicationExceptionHandler(registers_t *regs);
extern void securityExceptionHandler(registers_t *regs);



extern void randomGoofyInterruptsHandler(registers_t *regs);
extern void systemCallHandler(registers_t *regs);