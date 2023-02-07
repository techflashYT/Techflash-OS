#include <kernel/hardware/CPU/x86Setup.h>
#include <kernel/exceptions.h>

void initExceptions() {
	registerInterruptHandler(0,  &zeroDivHandler);
	registerInterruptHandler(1,  &debugHandler);
	registerInterruptHandler(2,  &NMIHandler);
	registerInterruptHandler(3,  &debugHandler);
	registerInterruptHandler(4,  &overflowHandler);
	registerInterruptHandler(5,  &boundRangeExceededHandler);
	registerInterruptHandler(6,  &invalidOpcodeHandler);
	registerInterruptHandler(8,  &doubleFaultHandler);
	registerInterruptHandler(11, &segmentNotPresetHandler);
	registerInterruptHandler(12, &stackSegmentFaultHandler);
	registerInterruptHandler(13, &generalProtectionFaultHandler);
	registerInterruptHandler(14, &pageFaultHandler);
	registerInterruptHandler(15, &reservedHandler);
	registerInterruptHandler(16, &FPUExceptionHandler);
	registerInterruptHandler(17, &alignmentCheckExceptionHandler);
	registerInterruptHandler(18, &machineCheckExceptionHandler);
	registerInterruptHandler(19, &SSEExceptionHandler);
	registerInterruptHandler(20, &virtualizationExceptionHandler);
	registerInterruptHandler(21, &controlProtectionExceptionHandler);
	registerInterruptHandler(22, &reservedHandler);
	registerInterruptHandler(23, &reservedHandler);
	registerInterruptHandler(24, &reservedHandler);
	registerInterruptHandler(25, &reservedHandler);
	registerInterruptHandler(26, &reservedHandler);
	registerInterruptHandler(27, &reservedHandler);
	registerInterruptHandler(28, &hypervisorInjectionExceptionHandler);
	registerInterruptHandler(29, &VMMCommunicationExceptionHandler);
	registerInterruptHandler(30, &securityExceptionHandler);
	registerInterruptHandler(31, &reservedHandler);
}