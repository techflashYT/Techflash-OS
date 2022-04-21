#include "inc/main.hpp"
extern "C" {
	#include "drivers/keyboard/inc/main.h"
	Keyboard keyboard;
	#include "../inc/util.h"
	#include "../cstdlib/defs.h"
}
extern "C" {
	extern void kernelMain();
}
void kernelMain() {
	// setupRealIDT();
	// if (!pciInit()) {
	// 	print((const uint_fast8_t*)"FATAL ERROR: PCI INITIALIZATION FAILED", 0, 1, RED);
	// }
	// if (!usbInit()) {
	// 	print((const uint_fast8_t*)"FATAL ERROR: USB INITIALIZATION FAILED", 0, 1, RED);
	// }
	terminal.cursor.position.x = 13;
	terminal.cursor.position.y = 0;
	printf(".");
	halt();
	printf("\033[{0};{9}H.\r\n");
	printf("\033[1;32mKernel Loaded\r\n");
	printf("\033[37mWelcome to Techflash OS \033[36m%s\033[37m!\r\n", VERSION);
	printf("\033[0m> "); // Print a prompt
	terminal.cursor.enable(15, 15);
	terminal.cursor.position.change(1, 3);
	for (;;) {
		keyboard.handler();
		putc(keyboard.getChar(), stdout);
	}
	softHalt();
	printf("\r\nThis broke through the softHalt!");
	halt();
	return;
}
