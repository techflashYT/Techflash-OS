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

bool alreadyPanicing = false;
bool mentionDualPanic = false;
void panic(const char* message, registers_t regs) {
	if (alreadyPanicing) {
		mentionDualPanic = true;
		return;
	}
	alreadyPanicing = true;
	kernTTY.cursorX = 0;
	kernTTY.cursorY = 0;
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Warray-bounds"
	#pragma GCC diagnostic ignored "-Wstringop-overflow"
	kernTTY.setBackground(0x0022DD); // light blue
	#pragma GCC diagnostic pop
	kernTTY.color = vga.colors.white;
	puts(
"Techflash OS has run into a problem it couldn't handle and needs to restart.\r\n\
Please report this to Techflash at https://github.com/techflashYT/Techflash-OS\r\n\
Please give the following information in the bug report:\r\n\
Error: "
	);
// Commented out because printf is busted
/*
	printf(
"%s\r\n\
CPU Registers:\r\n\
	RAX: %x\r\n\
	RBX: %x\r\n\
	RCX: %x\r\n\
	RDX: %x\r\n\
	RBP: %x\r\n\
	RSP: %x\r\n\
	RSI: %x\r\n\
	RDI: %x", message, regs.rax, regs.rbx, regs.rcx, regs.rdx, regs.rbp, regs.rsp, regs.rsi, regs.rdi);
*/
	puts(message);
	puts("\r\nCPU Registers:\r\n    RAX: ");
	char *itoaBuf = "";
	itoa(regs.rax, itoaBuf, 16);
	puts(itoaBuf);
	puts("\r\n    RBX: ");
	itoa(regs.rbx, itoaBuf, 16);
	puts(itoaBuf);
	puts("\r\n    RCX: ");
	itoa(regs.rcx, itoaBuf, 16);
	puts(itoaBuf);
	puts("\r\n    RDX: ");
	itoa(regs.rdx, itoaBuf, 16);
	puts(itoaBuf);
	puts("\r\n    RBP: ");
	itoa(regs.rbp, itoaBuf, 16);
	puts(itoaBuf);
	puts("\r\n    RSP: ");
	itoa(regs.userRsp, itoaBuf, 16);
	puts(itoaBuf);
	puts("\r\n    RSI: ");
	puts("\r\n    RSI: ");
	itoa(regs.rsi, itoaBuf, 16);
	puts(itoaBuf);
	puts("\r\n    RDI: ");
	itoa(regs.rdi, itoaBuf, 16);
	puts(itoaBuf);
	if (mentionDualPanic) {
		puts("\r\nAdditionally, an error has occurred during the printing of this message.\r\n");
	}

	asm volatile (
		"cli\n"
		"hlt"
	);
}