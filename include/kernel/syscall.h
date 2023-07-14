#include <kernel/hardware/CPU/x86Setup.h>
extern void (*syscallHandlers[0xFFFF])();
extern void initSyscalls();
extern void systemCallHandler(registers_t *regs);