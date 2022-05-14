#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/assert.h>
#include <kernel/sourceFileInfo.h>
static sourceFileInfo fileInfo = {
	.fileName       = "kernel/kernel.c",
	.lastEditor     = "Techflash",
	.lastEditDate   = "May 14th, 2022",
	.lastEditReason = "Add beginning of handler, and rename the function from kernel_main to kernelMain.",
	.versionMajor   = 0,
	.versionMinor   = 0,
	.versionPatch   = 2
};
void kernelMain(void) {
	terminalInit();
	printf("Hello, kernel World!\r\n");
	assert((const char*)"The kernel has reached the end of execution.  The system will now halt.", fileInfo, __LINE__);
	for (;;) {
		
	}
}
