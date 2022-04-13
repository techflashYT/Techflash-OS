#include "../cstdlib/defs.h"
void halt();
void kernelPanic(const char message[]) {
	printf("\033[1;31mKERNEL PANIC: %s", message);
	halt();
}
