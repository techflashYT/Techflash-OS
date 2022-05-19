#include <stdio.h>
#include <assert.h>

#include <kernel/tty.h>
#include <kernel/sourceFileInfo.h>
#include <kernel/timer.h>
#include <kernel/GDT.h>
#include <kernel/debug.h>
static sourceFileInfo fileInfo = {
	.fileName       = "kernel/kernel.c",
	.lastEditor     = "Techflash",
	.lastEditDate   = "May 14th, 2022",
	.lastEditReason = "Add beginning of handler, and rename the function from kernel_main to kernelMain.",
	.versionMajor   = 0,
	.versionMinor   = 0,
	.versionPatch   = 2
};
void kernelMain(char* GrubArgs) {
	// Retrieve the arguments from GRUB that were passed in from the assembly earlier.
	// TODO: This ^
	terminalInit();
	printf("[ %d.%d ] Terminal initialized\r\n", timer.now.seconds(), timer.now.milliseconds());
	GDTinit();
	printf("[ %d.%d ] Global Descriptor Table initialized\r\n", timer.now.seconds(), timer.now.milliseconds());
	printf("[ %d.%d ] KERN_ARGS: %s\r\n", timer.now.seconds(), timer.now.milliseconds(), GrubArgs);
	printf("Welcome to Techflash OS v%d.%d.%d!\r\n", fileInfo.versionMajor, fileInfo.versionMinor, fileInfo.versionPatch);
	printf("> ");
	kernelDebug((const char*)"The kernel has reached the end of execution.  The system will now halt.", fileInfo, __LINE__);
	for (;;) {
		
	}
}
