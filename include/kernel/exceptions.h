#include <kernel/panic.h>
#include <stdio.h>
void zeroDivHandler(registers_t *regs);
void debugHandler(registers_t *regs);
void NMIHandler(registers_t *regs);
void overflowHandler(registers_t *regs);
void boundRangeExceededHandler(registers_t *regs);
void invalidOpcodeHandler(registers_t *regs);
void doubleFaultHandler(registers_t *regs);
void segmentNotPresetHandler(registers_t *regs);
void stackSegmentFaultHandler(registers_t *regs);
void generalProtectionFaultHandler(registers_t *regs);
void pageFaultHandler(registers_t *regs);
void reservedHandler(registers_t *regs);
void FPUExceptionHandler(registers_t *regs);
void alignmentCheckExceptionHandler(registers_t *regs);
void machineCheckExceptionHandler(registers_t *regs);
void SSEExceptionHandler(registers_t *regs);
void virtualizationExceptionHandler(registers_t *regs);
void controlProtectionExceptionHandler(registers_t *regs);
void hypervisorInjectionExceptionHandler(registers_t *regs);
void VMMCommunicationExceptionHandler(registers_t *regs);
void securityExceptionHandler(registers_t *regs);