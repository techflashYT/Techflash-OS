#include "../inc/util.hpp"
#include "inc/main.hpp"
#include "drivers/keyboard/inc/main.hpp"
Keyboard keyboard;
extern "C" {
	extern void printTest();
}
void kernelMain() {
	// setupRealIDT();
	// pciInit();
	// if (!pciInit()) {
	// 	print((const uint_fast8_t*)"FATAL ERROR: PCI INITIALIZATION FAILED", 0, 1, RED);
	// }
	// if (!usbInit()) {
	// 	print((const uint_fast8_t*)"FATAL ERROR: USB INITIALIZATION FAILED", 0, 1, RED);
	// }
	kprintf(9, 0, DARK_GRAY, ".");
	kprintf(0, 1, GREEN, "Kernel Loaded");
	kprintf(0, 2, WHITE, "Welcome to Techflash OS");
	kprintf(24, 2, LIGHT_CYAN, VERSION);
	kprintf(30, 2, WHITE, "!");
	kprintf(0, 3, DARK_GRAY, "> "); // Print a prompt
	terminal.cursor.enable(15, 15);
	terminal.cursor.position.change(1, 3);
	keyboard.handler();
	softHalt();
	kprintf(0, 4, GREEN, "This broke through the softHalt!");
	halt();
	return;
}
