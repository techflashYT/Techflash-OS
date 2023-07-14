#include <kernel/hardware/CPU/x86Setup.h>
extern __attribute__((noreturn)) void panic(const char* message, registers_t *regs);