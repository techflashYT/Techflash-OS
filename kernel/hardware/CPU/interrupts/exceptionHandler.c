#include <kernel/panic.h>
#include <stdbool.h>
#include <kernel/misc.h>
__attribute__((noreturn)) void exceptionHandler(void);
void exceptionHandler() {
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wuninitialized"
	DUMPREGS
	#pragma GCC diagnostic pop
	panic("exceptionHandler(): Unhandled exception in kernel code!", regs);
	while (true) {};
}