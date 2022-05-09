#include <stdio.h>

#include <kernel/tty.h>

void kernel_main(void) {
	terminal_initialize();
	return; // Return to lock the CPU
	printf("Hello, kernel World!\n");
}
