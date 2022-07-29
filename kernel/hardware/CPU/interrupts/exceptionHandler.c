#include <kernel/panic.h>
#include <stdbool.h>
__attribute__((noreturn)) void exceptionHandler(void);
void exceptionHandler() {
	panic("exceptionHandler(): Unhandled exception in kernel code!");
	while (true) {};
}