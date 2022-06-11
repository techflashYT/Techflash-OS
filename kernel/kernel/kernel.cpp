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
	#include <kernel/bootDisplay.h>
	#include <math.h>

	extern int _testasm();
	void kernelMain(uint32_t magicnum, uint32_t mutliboot2info);
}
#include <kernel/tty.hpp>
uint8_t bootProgress = 0;
void kernelMain(uint32_t magicnum, uint32_t mutliboot2info) {
	terminalRow++;
	outb(KBD_PORT, 0xED);
	outb(KBD_PORT, KBD_NONE); // Turn all LEDs off	
	outb(KBD_PORT, 0xED);
	outb(KBD_PORT, KBD_NUMLOCK); // Turn on numlock
	struct multiboot_tag *mbtag;
	
	unsigned int __attribute__ ((unused)) size = 0;
	terminalInit();
	terminalPutEntryAt(0xFE, VGA_COLOR_LIGHT_BLUE, 33, 0);
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
	// Seperate the cmdline into the args array, seperating on spaces
	uint16_t numargs = 0;
	uint16_t y = 0;
	if (strlen(cmdline) > 0) {
		numargs = 1;
		for (size_t i = 0; i < strlen(cmdline); i++) {
			if (cmdline[i] == ' ') {
				numargs++;
				y = 0;
			}
			else {
				args[numargs][y] = cmdline[i];
				y++;
			}
		}
		for (uint16_t i = 0; i < numargs; i++) {
			printf(args[i]);
			/* TODO: More args to be handled:
			loglevel=X: 0: Debug+   1: Info+  2: Warning+  3: Error+   4. Nothing at all, not even Errors, only if you get a kernel panic does that show up
			verbose: Equivalent to loglevel=0
			*none*: Equivalent to loglevel=2
			quiet: Equivalent to loglevel=3
			silent: Equivalent to loglevel=4
			TODO: Think of more useful ones to put here
			*/
		}
	}
	terminalClear();
	bootDisplayMakeProgressBar();
	printf("[ %d.%d ] Initializing the Global Descriptor Table...\r\n", 0, 0);
	bootDisplayMakeBrackets(2 + numargs);
	GDTinit();
	bootProgress++;
	bootDisplayProgressBarUpdate();
	bootDisplayOK(2 + numargs);

	printf("[ %d.%d ] Initializing the programable interrupt timer...\r\n", 0, 0);
	bootDisplayMakeBrackets(3 + numargs);
	// pitInit();
	bootProgress++;
	bootDisplayProgressBarUpdate();
	bootDisplayOK(3 + numargs);

	printf("[ %d.%d ] Testing that calling a handwritten ASM function works...\r\n", 0, 0);
	bootDisplayMakeBrackets(4 + numargs);
	int ret = _testasm();
	if (ret == 438) {
			bootDisplayOK(4 + numargs);
	}
	else {
		bootDisplayFAIL(4 + numargs);
		bootDisplayProgressBarFail();
		panic("Handwritten ASM test returned a non-438 value");
	}
	bootProgress++;
	bootDisplayProgressBarUpdate();
	terminalWrite("Welcome to", sizeof("Welcome to"));
	terminalSetColor(VGA_COLOR_WHITE);
	terminalWrite("Techflash OS", sizeof("Techflash OS"));
	terminalSetColor(VGA_COLOR_LIGHT_GRAY);
	terminalPutchar('v');
	terminalSetColor(VGA_COLOR_CYAN);
	printf("%d.%d.%d", CONFIG_KERN_VERSION_MAJOR, CONFIG_KERN_VERSION_MINOR, CONFIG_KERN_VERSION_PATCH);
	terminalSetColor(VGA_COLOR_LIGHT_GRAY);
	#ifdef __GNUC__
		printf("!\r\nCompiled on");
		terminalSetColor(VGA_COLOR_LIGHT_BLUE);
		terminalWriteString(__DATE__);
		terminalSetColor(VGA_COLOR_LIGHT_GRAY);
		terminalWriteString(", ");
		terminalSetColor(VGA_COLOR_LIGHT_BLUE);
		terminalWriteString(__TIME__);
		terminalSetColor(VGA_COLOR_LIGHT_GRAY);
		terminalWriteString(" using ");
		terminalSetColor(VGA_COLOR_GREEN);
		terminalWriteString((CONFIG_BITS == 64 ? "x86_64-elf-gcc" : "i686-elf-gcc"));
		terminalSetColor(VGA_COLOR_LIGHT_GRAY);
		terminalWriteString(" version ");
		printf("%d.%d.%d\r\n", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
	#else
		printf("!\r\nCompiled on");
		terminalSetColor(VGA_COLOR_LIGHT_BLUE);
		terminalWriteString(__DATE__);
		terminalSetColor(VGA_COLOR_LIGHT_GRAY);
		terminalWriteString(", ");
		terminalSetColor(VGA_COLOR_LIGHT_BLUE);
		terminalWriteString(__TIME__);
		terminalSetColor(VGA_COLOR_LIGHT_GRAY);
		terminalWriteString(" using ");
		terminalSetColor(VGA_COLOR_RED);
		terminalWriteString("*unkown compiler*");
		terminalSetColor(VGA_COLOR_LIGHT_GRAY);
	#endif
	printf("Lets test your VGA colors, do you see a rainbow of colors here?\r\n");
	terminalColumn = 32; // FIXME: This isn't centered!
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
	terminalColumn = 46;
	terminalRow++;
	putchar(0x1E);
	terminalColumn = 46;
	terminalRow++;
	putchar('|');
	terminalColumn = 46;
	terminalRow++;
	putchar('|');
	terminalRow++;
	terminalColumn = 36;
	printf("Should be black (invisible)\r\n");
	// printf("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789`~!@#$%%^&*()_-+={}|[]\\:\";'<>?,./\r\n");
	// kernelDebug((const char*)"The kernel has reached the end of execution.  The system will now halt.", fileInfo, __LINE__);
	terminalSetColor(VGA_COLOR_LIGHT_GRAY);
	for (;;) {
		printf("Hey!  Sleep is working!\r\n");
		// pitWait(10);
	}
}
