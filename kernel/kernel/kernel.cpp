extern "C" {
	#include <stdio.h>
	#include <string.h>
	#include <stdbool.h>
	#include <kernel/arch/i386/vga.h>
	#include <kernel/arch/i386/GDT.h>
	#include <kernel/multiboot2.h>
	#include <kernel/panic.h>
	#include <kernel/arch/i386/pit.h>
	#include <kernel/arch/i386/io.h>
	#include <kernel/arch/i386/kbd.h>
	#include <kernel/arch/i386/ISR.h>
	#include <kernel/bootDisplay.h>
	#include <math.h>

	extern int _testasm();
	void kernelMain(uint32_t magicnum, uint32_t mutliboot2info);
}
#include <kernel/tty.hpp>
void kernelMain(uint32_t magicnum, uint32_t mutliboot2info) {
	outb(KBD_PORT, 0xED);
	outb(KBD_PORT, KBD_NONE); // Turn all LEDs off	
	outb(KBD_PORT, 0xED);
	outb(KBD_PORT, KBD_NUMLOCK); // Turn on numlock
	struct multiboot_tag *mbtag;
	
	unsigned int __attribute__ ((unused)) size = 0;
	terminalInit();
	terminalPutEntryAt(0xFE, VGA_COLOR_LIGHT_BLUE, 32, 0);
	// __asm__("cli\nhlt");
	printf("[ %d.%d ] Terminal initialized\r\n", 0, 0);
	bootDisplayMakeBrackets(1);
	bootDisplayOK(1);
	if (magicnum != MULTIBOOT2_BOOTLOADER_MAGIC) {
		printf("Invalid multiboot2 magic number (not in hex sorry: %d)!\r\n", magicnum);
		panic("See above message");
	}
	if (mutliboot2info & 7) {
		printf("Unaligned MBI (not in hex sorry: %d)!\r\n", magicnum);
		panic("See above message");
	}
	size = *(unsigned int *) mutliboot2info;
	printf("[ %d.%d ] Initializing the Global Descriptor Table...\r\n", 0, 0);
	bootDisplayMakeBrackets(2);
	GDTinit();
	bootDisplayOK(2);

	printf("[ %d.%d ] Initializing the programable interrupt timer...\r\n", 0, 0);
	bootDisplayMakeBrackets(3);
	pitActivateChannel(0, 60);
	bootDisplayOK(3);
	printf("[ %d.%d ] Testing that calling a handwritten ASM function works...\r\n", 0, 0);
	bootDisplayMakeBrackets(4);
	int ret = _testasm();
	if (ret == 438) {
		bootDisplayOK(4);
	}
	else {
		bootDisplayFAIL(4);
		panic("Handwritten ASM test returned a non-438 value");
	}
	printf("[ %d.%d ] Initializing Interrupt Service Routines...\r\n", 0, 0);
	bootDisplayMakeBrackets(5);
	isrInstall();
	bootDisplayOK(5);
	mbtag = (struct multiboot_tag *) (mutliboot2info + 8);
	mbtag = (struct multiboot_tag *) ((multiboot_uint8_t *) mbtag + ((mbtag->size + 7) & ~(uint32_t)7));
	const char *cmdline = {((struct multiboot_tag_string *) mbtag)->string};
	printf("[ %d.%d ] KERN_ARGS: %s\r\n", 0, 0, strcmp(cmdline, NULL) ? cmdline : "NONE");
	const int root = (int)sqrt(CONFIG_KERN_MAXARGS);
	printf("sqrt of max_args: %d\r\n", root);
	#ifdef __cplusplus
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wvla"
	#endif
	char args[root][root];
	#ifdef __cplusplus
	#pragma GCC diagnostic pop
	#endif
	(void)args;
	for (uint16_t x = 0; x < root; x++) {
		for (uint16_t y = 0; y < root; y++) {
			// Mental note: Don't forget to re-check that you aren't running off the end of the array by like 65,280 bytes.  It's not a good idea
			args[x][y] = '\0';  // Zero out the string so we are sure that we aren't garbling everything
		}
	}
	terminalWrite("Welcome to ", sizeof("Welcome to "));
	terminalSetColor(VGA_COLOR_WHITE);
	terminalWrite("Techflash OS ", sizeof("Techflash OS "));
	terminalSetColor(VGA_COLOR_LIGHT_GRAY);
	terminalPutchar('v');
	terminalSetColor(VGA_COLOR_CYAN);
	printf("%d.%d.%d", CONFIG_KERN_VERSION_MAJOR, CONFIG_KERN_VERSION_MINOR, CONFIG_KERN_VERSION_PATCH);
	terminalSetColor(VGA_COLOR_LIGHT_GRAY);
	printf("!\r\n");
	printf("Lets test your VGA colors, do you see a rainbow of colors here?\r\n");
	terminalColumn = 24;
	terminalSetColor(VGA_COLOR_LIGHT_RED);
	putchar('#');
	terminalSetColor(VGA_COLOR_RED);
	putchar('#');
	terminalSetColor(VGA_COLOR_BROWN);
	putchar('#');
	terminalSetColor(VGA_COLOR_YELLOW);
	putchar('#');
	terminalSetColor(VGA_COLOR_LIGHT_GREEN);
	putchar('#');
	terminalSetColor(VGA_COLOR_GREEN);
	putchar('#');
	terminalSetColor(VGA_COLOR_BLUE);
	putchar('#');
	terminalSetColor(VGA_COLOR_LIGHT_BLUE);
	putchar('#');
	terminalSetColor(VGA_COLOR_CYAN);
	putchar('#');
	terminalSetColor(VGA_COLOR_LIGHT_CYAN);
	putchar('#');
	terminalSetColor(VGA_COLOR_LIGHT_MAGENTA);
	putchar('#');
	terminalSetColor(VGA_COLOR_MAGENTA);
	putchar('#');
	terminalSetColor(VGA_COLOR_LIGHT_GRAY);
	putchar('#');
	terminalSetColor(VGA_COLOR_DARK_GRAY);
	putchar('#');
	terminalSetColor(VGA_COLOR_BLACK);
	putchar('#');
	terminalSetColor(VGA_COLOR_WHITE);
	putchar('#');
	terminalColumn = 38;
	terminalRow++;
	putchar(0x1E);
	terminalColumn = 38;
	terminalRow++;
	putchar('|');
	terminalColumn = 38;
	terminalRow++;
	putchar('|');
	terminalRow++;
	terminalColumn = 30;
	printf("Should be black (invisible)");
	// printf("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789`~!@#$%%^&*()_-+={}|[]\\:\";'<>?,./\r\n");
	// kernelDebug((const char*)"The kernel has reached the end of execution.  The system will now halt.", fileInfo, __LINE__);
	for (;;) {
	}
}
