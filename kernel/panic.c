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

char *rax;
char *rbx;
char *rcx;
char *rdx;
char *rbp;
char *rsp;
char *rsi;
char *rdi;
char *rip;
char *cr2;
char *intNo;

extern void padNumTo(char *src, uint8_t padding);
bool alreadyPanicing = false;
bool mentionDualPanic = false;
void panic(const char* message, registers_t *regs) {
	// if it's true: we already paniced, and it paniced during that as well, now it did it again, give up to avoid making this bombshell even worse.
	if (!mentionDualPanic) {
		if (alreadyPanicing) {
			mentionDualPanic = true;
			return;
		}
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

	rax = malloc(17);
	rbx = malloc(17);
	rcx = malloc(17);
	rdx = malloc(17);
	rbp = malloc(17);
	rsp = malloc(17);
	rsi = malloc(17);
	rdi = malloc(17);
	rip = malloc(17);
	cr2 = malloc(17);
	intNo = malloc(17);

	uint64_t cr2temp = 0;
	asm("movq %%rbp, %0\r\n" : "=r" (cr2temp) : );\

	utoa(regs->rax, rax, 16);
	padNumTo(rax, 16);
	utoa(regs->rbx, rbx, 16);
	padNumTo(rbx, 16);
	utoa(regs->rcx, rcx, 16);
	padNumTo(rcx, 16);
	utoa(regs->rdx, rdx, 16);
	padNumTo(rdx, 16);
	utoa(regs->rbp, rbp, 16);
	padNumTo(rbp, 16);
	utoa(regs->userRsp, rsp, 16);
	padNumTo(rsp, 16);
	utoa(regs->rsi, rsi, 16);
	padNumTo(rsi, 16);
	utoa(regs->rdi, rdi, 16);
	padNumTo(rdi, 16);
	utoa(regs->rip, rip, 16);
	padNumTo(rip, 16);
	utoa(regs->intNo, intNo, 16);
	padNumTo(intNo, 16);
	utoa(cr2temp, cr2, 16);
	padNumTo(cr2, 16);
	
	printf (
		"%s\r\n"
		"CPU Registers:\r\n"
		"	RAX: 0x%s\r\n"
		"	RBX: 0x%s\r\n"
		"	RCX: 0x%s\r\n"
		"	RDX: 0x%s\r\n"
		"	RBP: 0x%s\r\n"
		"	RSP: 0x%s\r\n"
		"	RSI: 0x%s\r\n"
		"	RDI: 0x%s\r\n"
		"	RIP: 0x%s\r\n"
		"	CR2: 0x%s\r\n"
		"	Interrupt Number: 0x%s\r\n",
		message,
		rax,
		rbx,
		rcx,
		rdx,
		rbp,
		rsp,
		rsi,
		rdi,
		rip,
		cr2,
		intNo
	);
	if (mentionDualPanic) {
		puts("Additionally, an error has occurred during the printing of this message.\r\n");
	}
	while (true) {
		asm volatile (
			"cli\n"
			"hlt"
		);
	}
}
