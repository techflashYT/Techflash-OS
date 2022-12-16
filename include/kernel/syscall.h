#include <kernel/hardware/CPU/ISR.h>
void initSyscalls();
extern void (*syscallHandlers[0xFFFF])();
void systemCallHandler(registers_t *regs);