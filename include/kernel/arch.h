#ifdef __x86_64__
#include <kernel/arch/x86/registers.h>
#else
typedef void registers_t;
#endif

extern void ARCH_Init();
extern void ARCH_PrintRegs(registers_t *regs);