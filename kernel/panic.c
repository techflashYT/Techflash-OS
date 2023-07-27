#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <kernel/environment.h>
#include <kernel/graphics.h>
#include <kernel/tty.h>
#include <kernel/hardware/CPU/x86Setup.h>
#include <kernel/hardware/serial.h>
#include <kernel/stack.h>
#include <kernel/fs/tar.h>
#include <kernel/symTable.h>
#include <kernel/custom.h>
static uint64_t cr2;


static void regs(volatile registers_t *r) {
	puts("\r\n\e[1m\e[36mCPU Registers ");
	TTY_Bold = false;
	TTY_Color = colors.error;
	puts("(include these in the bug report!!!!)");
	TTY_Color = colors.vga.lcyan;
	TTY_Bold = true;
	putchar(':');
	TTY_Bold = false;
	TTY_Color = colors.warn;
	// cr2 isn't pushed already, handle it manually
	asm("movq %%cr2, %0\r\n" : "=r" (cr2) : );
	printf (
		"\r\n	General Purpose Regs:\r\n"
		"\e[1m"
		"		\e[36mRAX: \e[37m%p	\e[36mRBX: \e[37m%p	\e[36mRCX: \e[37m%p\r\n"
		"		\e[36mRDX: \e[37m%p	\e[36mRSI: \e[37m%p	\e[36mRDI: \e[37m%p\r\n"
		"		\e[36mR8:  \e[37m%p	\e[36mR9:  \e[37m%p	\e[36mR10: \e[37m%p\r\n"
		"		\e[36mR11: \e[37m%p	\e[36mR12: \e[37m%p	\e[36mR13: \e[37m%p\r\n"
		"		\e[36mR14: \e[37m%p	\e[36mR15: \e[37m%p\r\n",
		(void *)r->rax, (void *)r->rbx, (void *)r->rcx,
		(void *)r->rdx, (void *)r->rsi, (void *)r->rdi,
		(void *)r->r8,  (void *)r->r9,  (void *)r->r10,
		(void *)r->r11, (void *)r->r12, (void *)r->r13,
		(void *)r->r14, (void *)r->r15
	);
	TTY_Bold = false;
	TTY_Color = colors.warn;
	printf (
		"	Pointer regs:\r\n"
		"		\e[1m\e[36mRBP: \e[37m%p	\e[36mRSP: \e[37m%p	\e[36mRIP: \e[37m%p\r\n",
		(void *)r->rbp, (void *)r->userRsp, (void *)r->rip
	);
	TTY_Bold = false;
	TTY_Color = colors.warn;
	printf(
		"	Other regs:\r\n"
		"		\e[1m\e[36mCR2: \e[37m%p", (void *)cr2
	);
	if (r->intNo != 0) {
		printf(
			"	\e[36mInterrupt Number: \e[37m%p\r\n		\e[36mError Code: \e[37m%p",
			(void *)r->intNo, (void *)r->errCode
		);
	}
}
uint8_t panicNum = 0;
__attribute__((noreturn)) void panic(const char* message, volatile registers_t *r) {
	// while(true) {};
	if (panicNum == 2) {
		while (true) {asm("cli;hlt");}
	}
	else if (panicNum == 1) {
		goto panic2;
	}

	TTY_CursorX = 0;
	TTY_CursorY = 0;
	TTY_SetBackground(0x0062A8FF); // light blue
	TTY_Color = colors.vga.white;

	serial.writeString(SERIAL_PORT_COM1, "Kernel Panic!  Error: ");
	serial.writeString(SERIAL_PORT_COM1, message);
	serial.writeString(SERIAL_PORT_COM1, "\r\nPlease see framebuffer for debugging info.\r\n");
	// Puts is slightly faster here since there's no need to check for format specifiers
	puts(
		"Techflash OS has run into a problem it couldn't handle and needs to restart.\r\n"
		"Please report this to Techflash at https://github.com/techflashYT/Techflash-OS\r\n"
		"Please give the following information "
	);
	TTY_Color = colors.error;
	puts("EXACTLY AS SHOWN BELOW");
	TTY_Color = colors.vga.white;
	puts(" in the bug report:\r\n");
	
	TTY_Color = colors.error;
	printf("Error: \e[1m\e[33m%s", message);

	regs(r);
panic2:
	panicNum++;

	if (panicNum == 2) {
		printf(
			"\r\n\nAdditionally, an error has occurred during the printing of this message.\r\n"
			"Error: \e[1;33m%s\e[0m\r\n",
			message
		);
		regs(r);
		puts("Halting just in case to prevent an infinite loop");
		asm("cli;hlt");
	}
	/* deallocate the str after */ {
		char str[76];
		sprintf(str, "\r\n============ STACK TRACE ============\r\n");
		puts(str);
		serial.writeString(SERIAL_PORT_COM1, str);

		sprintf(str, "NOTE: Starting from most recently called function, ending at entry point.\r\n");
		puts(str);
		serial.writeString(SERIAL_PORT_COM1, str);
	}
	// stack trace
	void **trace = stackTrace(9);
	symbolConvInfo_t info;
	info.name = "nonsense";
	info.offset = 0;


	for (uint64_t i = 0; trace[i] != 0; i++) {
		getSymbolByAddress(trace[i], &info);
		// basically prints `2: 0xDEADBEEF12345678 [someFunc+69420]` with very fancy colors
		TTY_Bold = false;
		TTY_Color = colors.warn;

		printf("%ld: \e[1m\e[37m%p \e[36m[\e[33m%s\e[37m+\e[31m%ld\e[36m]\e[37m\r\n", i, trace[i], info.name, info.offset);
	}

	serial.writeString(SERIAL_PORT_COM1, "done\r\n");
	serial.writeString(SERIAL_PORT_COM1, "halting...\r\n");
	while (true) {
		asm volatile (
			"cli\n"
			"hlt"
		);
	}
}
