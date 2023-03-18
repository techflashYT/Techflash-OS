#include <kernel/hardware/CPU/x86Setup.h>
__attribute__((noreturn)) void panic(const char* message, registers_t *regs);