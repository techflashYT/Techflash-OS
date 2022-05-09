#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/assert.h>
void kernel_main(void) {
	terminal_initialize();
	printf("Hello, kernel World!\r\n");
	assert("The kernel has reached the end of execution.  The system will now halt.", __FILE__, __LINE__);
	return; // Return to lock the CPU
}
