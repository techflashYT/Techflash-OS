#include <kernel/panic.h>
#include <stdbool.h>
#include <kernel/misc.h>
__attribute__((noreturn)) void exceptionHandler(void);
void exceptionHandler() {
	DUMPREGS
	panic("exceptionHandler(): Unhandled exception in kernel code!", regs);
	while (true) {};
}