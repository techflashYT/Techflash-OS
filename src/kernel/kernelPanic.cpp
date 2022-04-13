#include "../shared.hpp"
#include "../inc/util.hpp"
extern "C" {
    #include "../cstdlib/defs.h"
}
u void kernelPanic(const char message[]) {
	printf("\033[1;31mKERNEL PANIC: %s", message);
	halt();
}
