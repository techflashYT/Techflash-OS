#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/sourceFileInfo.h>
#include <kernel/timer.h>
#include <kernel/GDT.h>
#include <kernel/debug.h>
#include <kernel/panic.h>

extern int _testasm();
static sourceFileInfo fileInfo = {
	.fileName       = "kernel/kernel.c",
	.lastEditor     = "Techflash",
	.lastEditDate   = "May 14th, 2022",
	.lastEditReason = "Add beginning of handler, and rename the function from kernel_main to kernelMain.",
	.versionMajor   = 0,
	.versionMinor   = 0,
	.versionPatch   = 2
};
void kernelMain(void* grubCMDline) {
	

	terminalInit();
	printf("[ %d.%d ] Terminal initialized\r\n", timer.now.seconds(), timer.now.milliseconds());
	GDTinit();
	printf("[ %d.%d ] Global Descriptor Table initialized\r\n", timer.now.seconds(), timer.now.milliseconds());
	printf("[ %d.%d ] Test that calling a handwritten ASM function works: ", timer.now.seconds(), timer.now.milliseconds());
	
	if (_testasm() != 0x12345678) {
		printf("FAILED\r\n");
		panic("Handwritten ASM test returned a non-136 value");
	}
	else {
		printf("PASSED\r\n");
	}
	printf("[ %d.%d ] KERN_ARGS: %s\r\n", timer.now.seconds(), timer.now.milliseconds(), grubCMDline);
	// TODO: Handle arguments here
	printf("Welcome to Techflash OS v%d.%d.%d!\r\n", fileInfo.versionMajor, fileInfo.versionMinor, fileInfo.versionPatch);
	printf("> \r\n");
	kernelDebug((const char*)"The kernel has reached the end of execution.  The system will now halt.", fileInfo, __LINE__);
	for (;;) {
		
	}
}
