#include <stdio.h>

#include <kernel/tty.h>

void kernel_main(void) {
	return; // Return to lock the CPU
	terminal_initialize();
	printf("Hello, kernel World!\n");
}
