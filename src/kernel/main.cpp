#include "inc/main.hpp"
#include "../inc/util.hpp"
extern "C" {
	extern void printTest();
}
void kernelMain() {
	print((const uint_fast8_t*)".", 9, 0);
	print((const uint_fast8_t*)"Kernel Loaded", 0, 1, GREEN);
	halt();
	return;
}
