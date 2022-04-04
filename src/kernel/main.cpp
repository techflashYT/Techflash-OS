#include "inc/main.hpp"
#include "../inc/util.hpp"
extern "C" {
	extern void printTest();
}
void kernelMain() {
	for (uint_fast8_t i = 0; i <= 7; i++) {
		*vga_buffer++; // Increment the VGA buffer a little as to not overwrite the "Booting.." message
	}
	print((const uint_fast8_t*)".");
	halt();
	return;
}
