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
char* padNumTo(const char *src, uint8_t padding);
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
	kernTTY.setBackground(0x0062A8); // light blue
	kernTTY.color = vga.colors.white;
	char itoaBuf[32];

	puts(
"Techflash OS has run into a problem it couldn't handle and needs to restart.\r\n\
Please report this to Techflash at https://github.com/techflashYT/Techflash-OS\r\n\
Please give the following information in the bug report:\r\n\
Error: "
	); // Puts is slightly faster here since there's no need to check for format specifiers

	char rax[17];
	char rbx[17];
	char rcx[17];
	char rdx[17];
	char rbp[17];
	char rsp[17];
	char rsi[17];
	char rdi[17];
	char intNo[17];

	utoa(regs.rax, rax, 16);
	utoa(regs.rbx, rbx, 16);
	utoa(regs.rcx, rcx, 16);
	utoa(regs.rdx, rdx, 16);
	utoa(regs.rbp, rbp, 16);
	utoa(regs.userRsp, rsp, 16);
	utoa(regs.rsi, rsi, 16);
	utoa(regs.rdi, rdi, 16);
	utoa(regs.intNo, intNo, 16);
	printf(
"%s\r\n\
CPU Registers:\r\n\
    RAX: 0x%s\r\n\
    RBX: 0x%s\r\n\
    RCX: 0x%s\r\n\
    RDX: 0x%s\r\n\
    RBP: 0x%s\r\n\
    RSP: 0x%s\r\n\
    RSI: 0x%s\r\n\
    RDI: 0x%s\r\n\
    Interrupt Number: 0x%s",
	message,
	rax,
	rbx,
	rcx,
	rdx,
	rbp,
	rsp,
	rsi,
	rdi,
	intNo
	);
	if (mentionDualPanic) {
		puts("\r\nAdditionally, an error has occurred during the printing of this message.\r\n");
	}

	asm volatile (
		"cli\n"
		"hlt"
	);
}
