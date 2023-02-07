#include <kernel/hardware/CPU/x86Setup.h>
void initSyscalls();
extern void (*syscallHandlers[0xFFFF])();
void systemCallHandler(registers_t *regs);