#include "inc/main.hpp"
#include "../inc/util.hpp"
extern "C" {
	extern void printTest();
}
void kernelMain() {
	print((const uint_fast8_t*)".", 9, 0);
	print((const uint_fast8_t*)"Kernel Loaded", 0, 1, GREEN);
	print((const uint_fast8_t*)"Welcome to Techflash OS", 0, 2, WHITE);
	print((const uint_fast8_t*)VERSION, 24, 2, LIGHT_CYAN);
	print((const uint_fast8_t*)"!", 30, 2, WHITE);
	print((const uint_fast8_t*)"> ", 0, 3); // Print a prompt
	terminal.cursor.enable(15, 15);
	terminal.cursor.position.change(1, 3);
	halt();
	return;
}
