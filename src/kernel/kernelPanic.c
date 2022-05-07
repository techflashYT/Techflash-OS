#include "../cstdlib/defs.h"
#include <stdarg.h>
void halt();
void kernelPanic(const char message[], ...) {
	va_list args;
	va_start(args, message);
	char buffer[256];
	printf("\033[1;31mKERNEL PANIC: ");
	vasprintf(buffer, message, args);
	va_end(args);
	halt();
}
