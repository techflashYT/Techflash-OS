#include "../inc/util.hpp"
#include "inc/main.hpp"
#include "drivers/keyboard/inc/main.hpp"
Keyboard keyboard;
extern "C" {
	extern void kernelMain();
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
	printf(".");
	halt();
	printf("\033[{0};{9}H.\r\n");
	printf("\033[1;32mKernel Loaded\r\n");
	printf("\033[37mWelcome to Techflash OS \033[36m%s\033[37m!\r\n", VERSION);
	printf("\033[0m> "); // Print a prompt
	terminal.cursor.enable(15, 15);
	terminal.cursor.position.change(1, 3);
	for (uint_fast8_t i = 0; i < VGA_WIDTH; i++) {
		keyboard.handler();
		printf((const char*)keyboard.getChar());
	}
	softHalt();
	printf("\r\nThis broke through the softHalt!");
	halt();
	return;
}
