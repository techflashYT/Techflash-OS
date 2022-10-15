#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <kernel/environment.h>
#include <kernel/graphics.h>
#include <kernel/tty.h>
#include <kernel/hardware/CPU/regs.h>
#include <kernel/misc.h>
void padNumTo(char *src, uint8_t padding);
bool alreadyPanicing = false;
bool mentionDualPanic = false;
void panic(const char* message, registers_t *regs) {
	if (mentionDualPanic) {
		asm (
			"cli\n"
			"hlt\n"
		);
	}
	if (alreadyPanicing) {
		mentionDualPanic = true;
		return;
	}
	alreadyPanicing = true;
	kernTTY.cursorX = 0;
	kernTTY.cursorY = 0;
	kernTTY.setBackground(0x0062A8); // light blue
	kernTTY.color = vga.colors.white;

	puts(
"Techflash OS has run into a problem it couldn't handle and needs to restart.\r\n\
Please report this to Techflash at https://github.com/techflashYT/Techflash-OS\r\n\
Please give the following information in the bug report:\r\n\
Error: "
	); // Puts is slightly faster here since there's no need to check for format specifiers

	char reg1[17];
	char reg2[17];
	char reg3[17];


	utoa(regs->rax, reg1, 16);
	padNumTo(reg1, 16);
	utoa(regs->rbx, reg2, 16);
	padNumTo(reg2, 16);
	utoa(regs->rcx, reg3, 16);
	padNumTo(reg3, 16);
	printf (
		"%s\r\n"
		"CPU Registers:\r\n"
		"    RAX: 0x%s\r\n"
		"    RBX: 0x%s\r\n"
		"    RCX: 0x%s\r\n",
		message,
		reg1,
		reg2,
		reg3
	);
	utoa(regs->rdx, reg1, 16);
	padNumTo(reg1, 16);
	utoa(regs->rbp, reg2, 16);
	padNumTo(reg2, 16);
	utoa(regs->userRsp, reg3, 16);
	padNumTo(reg3, 16);
	
	printf (
		"    RDX: 0x%s\r\n"
		"    RBP: 0x%s\r\n"
		"    RSP: 0x%s\r\n",
		reg1,
		reg2,
		reg3
	);
	utoa(regs->rsi, reg1, 16);
	padNumTo(reg2, 16);
	utoa(regs->rdi, reg2, 16);
	padNumTo(reg2, 16);
	utoa(regs->intNo, reg3, 16);
	padNumTo(reg3, 16);
	printf (
		"    RSI: 0x%s\r\n"
		"    RDI: 0x%s\r\n"
		"    Interrupt Number: 0x%s\r\n",
		reg1,
		reg2,
		reg3
	);
	if (mentionDualPanic) {
		puts("Additionally, an error has occurred during the printing of this message.\r\n");
	}

	asm volatile (
		"cli\n"
		"hlt"
	);
}
