#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/assert.h>
void kernel_main(void) {
	terminalInit();
	printf("Hello, kernel World!\r\n");
	assert((const char*)"The kernel has reached the end of execution.  The system will now halt.", __FILE__, __LINE__);
	return; // Return to lock the CPU
}
