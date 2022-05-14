#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/assert.h>
#include <kernel/sourceFileInfo.h>
static sourceFileInfo fileInfo = {
	.fileName       = "kernel/kernel.c",
	.lastEditor     = "Techflash",
	.lastEditDate   = "May 9th, 2022",
	.lastEditReason = "Add fileInfo",
	.versionMajor   = 0,
	.versionMinor   = 0,
	.versionPatch   = 1
};
void kernel_main(void) {
	terminalInit();
	printf("Hello, kernel World!\r\n");
	assert((const char*)"The kernel has reached the end of execution.  The system will now halt.", fileInfo, __LINE__);
	return; // Return to lock the CPU
}
