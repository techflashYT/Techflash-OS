#ifdef __x86_64__
#include <kernel/arch/x86/registers.h>
#else
typedef void registers_t;
#endif

void __attribute__((noreturn)) panic(char *message, registers_t *regs);
