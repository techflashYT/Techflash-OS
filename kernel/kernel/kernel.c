#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../arch/i386/vga.h"
#include <kernel/tty.h>
#include <kernel/sourceFileInfo.h>
#include <kernel/GDT.h>
#include <kernel/debug.h>
#include <kernel/multiboot2.h>
#include <kernel/panic.h>

extern int _testasm();
static sourceFileInfo fileInfo = {
	.fileName       = "kernel/kernel.c",
	.lastEditor     = "Techflash",
	.lastEditDate   = "May 29th, 2022",
	.lastEditReason = "Add GRUB arguments, and add a super basic (as in absolutely nothing) arguments handler",
	.versionMajor   = CONFIG_KERN_VERSION_MAJOR,
	.versionMinor   = CONFIG_KERN_VERSION_MINOR,
	.versionPatch   = CONFIG_KERN_VERSION_PATCH
};
void kernelMain(unsigned long magicnum, unsigned long mutliboot2info) {
	struct multiboot_tag *mbtag;
	unsigned int __attribute__ ((unused)) size = 0;
	terminalInit();
	printf("[ %d.%d ] Terminal initialized\r\n", 0, 0);
	if (magicnum != MULTIBOOT2_BOOTLOADER_MAGIC) {
		printf("Invalid multiboot2 magic number (not in hex sorry: %d)!\r\n", magicnum);
		panic("See above message");
	}
	if (mutliboot2info & 7) {
		printf("Unaligned MBI (not in hex sorry: %d)!\r\n", magicnum);
		panic("See above message");
	}
	size = *(unsigned *) mutliboot2info;
	GDTinit();
	printf("[ %d.%d ] Global Descriptor Table initialized\r\n", 0, 0);
	printf("[ %d.%d ] Test that calling a handwritten ASM function works: ", 0, 0);
	
	if (_testasm() != 0x12345678) {
		printf("FAILED\r\n");
		panic("Handwritten ASM test returned a non-0x12345678 value");
	}
	else {
		printf("PASSED\r\n");
	}
	mbtag = (struct multiboot_tag *) (mutliboot2info + 8);
	// TODO: **GET THIS WORKING AGAIN!!!!**
	const char *cmdline = {((struct multiboot_tag_string *) mbtag)->string};
	printf("[ %d.%d ] KERN_ARGS: %s\r\n", 0, 0, strcmp(cmdline, NULL) ? cmdline : "NONE");
	for (uint16_t i = 0; i < CONFIG_KERN_MAXARGS; i++) {

	}
	terminalWriteString("Welcome to ");
	terminalSetColor(VGA_COLOR_WHITE);
	terminalWriteString("Techflash OS ");
	terminalSetColor(VGA_COLOR_LIGHT_GRAY);
	terminalPutchar('v');
	terminalSetColor(VGA_COLOR_CYAN);
	printf("%d.%d.%d", fileInfo.versionMajor, fileInfo.versionMinor, fileInfo.versionPatch);
	terminalSetColor(VGA_COLOR_LIGHT_GRAY);
	printf("!\r\n");
	printf("> \r\n");
	printf("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789`~!@#$%%^&*()_-+={}|[]\\:\";'<>?,./\r\n");
	// kernelDebug((const char*)"The kernel has reached the end of execution.  The system will now halt.", fileInfo, __LINE__);
	for (;;) {
		
	}
}
