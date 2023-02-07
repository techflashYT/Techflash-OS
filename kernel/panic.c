#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <kernel/environment.h>
#include <kernel/graphics.h>
#include <kernel/tty/tty.h>
#include <kernel/hardware/CPU/x86Setup.h>
#include <kernel/hardware/serial.h>
#include <kernel/misc.h>
#include <kernel/fs/tar.h>
static char *rax; static char *rbx; static char *rcx;
static char *rdx; static char *rsi; static char *rdi;
static char *r8;  static char *r9;  static char *r10;
static char *r11; static char *r12; static char *r13;
static char *r14; static char *r15;

static char *rbp; static char *rsp; static char *rip;

static char *cr2; static char *intNo;

static uint64_t cr2val;
static uint64_t r8val;  static uint64_t r9val;  static uint64_t r10val;
static uint64_t r11val; static uint64_t r12val; static uint64_t r13val;
static uint64_t r14val; static uint64_t r15val;

extern void padNumTo(char *src, uint8_t padding);

static bool alreadyPanicing = false;
static bool mentionDualPanic = false;

void panic(const char* message, registers_t *regs) {
	// if it's true: we panicked during a panic during a panic, the panic code is probably dead, just give up.
	if (!mentionDualPanic) {
		if (alreadyPanicing) {
			asm("cli; hlt");
			return;
		}
	}

	alreadyPanicing = true;
	kernTTY.cursorX = 0;
	kernTTY.cursorY = 0;
	kernTTY.setBackground(0x0062A8); // light blue
	kernTTY.color = colors.vga.white;

	serial.writeString(SERIAL_PORT_COM1, "Kernel Panic!  Error: ");
	serial.writeString(SERIAL_PORT_COM1, message);
	serial.writeString(SERIAL_PORT_COM1, "\r\nPlease see framebuffer for debugging info.\r\n");

	// Puts is slightly faster here since there's no need to check for format specifiers
	puts(
		"Techflash OS has run into a problem it couldn't handle and needs to restart.\r\n"
		"Please report this to Techflash at https://github.com/techflashYT/Techflash-OS\r\n"
		"Please give the following information "
	);
	kernTTY.color = colors.error;
	puts("EXACTLY AS SHOWN BELOW");
	kernTTY.color = colors.vga.white;
	puts(" in the bug report:\r\n");
	kernTTY.color = colors.error;
	puts("Error: ");
	kernTTY.color = colors.vga.yellow;
	puts(message);
	kernTTY.color = colors.vga.lcyan;
	puts("\r\nCPU Registers ");
	kernTTY.color = colors.error;
	puts("(include these in the bug report!!!!)");
	kernTTY.color = colors.vga.lcyan;
	putchar(':');
	kernTTY.color = colors.warn;
	puts("\r\n	General Purpose Regs:\r\n");

	rax = malloc(17); rbx = malloc(17); rcx = malloc(17);
	rdx = malloc(17); rsi = malloc(17); rdi = malloc(17);
	r8  = malloc(17); r9  = malloc(17); r10 = malloc(17);
	r11 = malloc(17); r12 = malloc(17); r13 = malloc(17);
	r14 = malloc(17); r15 = malloc(17);

	rbp = malloc(17); rsp = malloc(17); rip = malloc(17);

	cr2 = malloc(17); intNo = malloc(17);
	// cr2 isn't pushed already, handle it manually
	cr2val = 0;	asm("movq %%cr2, %0\r\n" : "=r" (cr2val) : );
	// grab r8-r15
	r8val  = 0;	asm("movq %%r8, %0\r\n"  : "=r" (r8val) : );
	r9val  = 0;	asm("movq %%r9, %0\r\n"  : "=r" (r9val) : );
	r10val = 0;	asm("movq %%r10, %0\r\n" : "=r" (r10val) : );
	r11val = 0;	asm("movq %%r11, %0\r\n" : "=r" (r11val) : );
	r12val = 0;	asm("movq %%r12, %0\r\n" : "=r" (r12val) : );
	r13val = 0;	asm("movq %%r13, %0\r\n" : "=r" (r13val) : );
	r14val = 0;	asm("movq %%r14, %0\r\n" : "=r" (r14val) : );
	r15val = 0;	asm("movq %%r15, %0\r\n" : "=r" (r15val) : );
	utoa(regs->rax, rax, 16);     padNumTo(rax, 16);
	utoa(regs->rbx, rbx, 16);     padNumTo(rbx, 16);
	utoa(regs->rcx, rcx, 16);     padNumTo(rcx, 16);

	utoa(regs->rdx, rdx, 16);     padNumTo(rdx, 16);
	utoa(regs->rsi, rsi, 16);     padNumTo(rsi, 16);
	utoa(regs->rdi, rdi, 16);     padNumTo(rdi, 16);

	utoa(r8val,  r8,  16);        padNumTo(r8,  16);
	utoa(r9val,  r9,  16);        padNumTo(r9,  16);
	utoa(r10val, r10, 16);        padNumTo(r10, 16);

	utoa(r11val, r11, 16);        padNumTo(r11, 16);
	utoa(r12val, r12, 16);        padNumTo(r12, 16);
	utoa(r13val, r13, 16);        padNumTo(r13, 16);


	utoa(r14val, r14, 16);        padNumTo(r14, 16);
	utoa(r15val, r15, 16);        padNumTo(r15, 16);

	utoa(regs->rbp, rbp, 16);     padNumTo(rbp, 16);
	utoa(regs->userRsp, rsp, 16); padNumTo(rsp, 16);
	utoa(regs->rip, rip, 16);     padNumTo(rip, 16);

	utoa(cr2val, cr2, 16);        padNumTo(cr2, 16);
	intNo[0] = '\0';
	if (intNo != 0) {
		utoa(regs->intNo, intNo, 16); padNumTo(intNo, 16);
	}
	
	kernTTY.color = colors.vga.white;
	printf (
		"		RAX: 0x%s	RBX: 0x%s	RCX: 0x%s\r\n"
		"		RDX: 0x%s	RSI: 0x%s	RDI: 0x%s\r\n"
		"		R8:  0x%s	R9:  0x%s	R10: 0x%s\r\n"
		"		R11: 0x%s	R12: 0x%s	R13: 0x%s\r\n"
		"		R14: 0x%s	R15: 0x%s\r\n",
		rax, rbx, rcx,
		rdx, rsi, rdi,
		r8,  r9,  r10,
		r11, r12, r13,
		r14, r15
	);
	kernTTY.color = colors.warn;
	puts("	Pointer regs:\r\n");
	kernTTY.color = colors.vga.white;
	printf(
		"		RBP: 0x%s	RSP: 0x%s	RIP: 0x%s\r\n",
		rbp, rsp, rip
	);
	kernTTY.color = colors.warn;
	puts("	Other regs:\r\n");
	kernTTY.color = colors.vga.white;
	puts("		CR2: 0x"); puts(intNo);
	if (regs->intNo != 0) {
		puts("		Interrupt Number: 0x"); puts(intNo); puts("\r\n");
	}
	if (mentionDualPanic) {
		puts("Additionally, an error has occurred during the printing of this message.\r\n");
	}
	// write panic_screen.sys to the fb
	serial.writeString(SERIAL_PORT_COM1, "dumping img to fb... ");

	uint8_t *filePtr = 0;
	size_t fileSize = readFile(((uint8_t *)bootboot.initrd_ptr), "panic_screen.sys", &filePtr);
	printf("\r\npanic_screen.sys filesize: %llu\r\nptr: %p\r\n", fileSize, filePtr);
	if (fileSize == 0) {
		puts("\r\nFailed to read panic_screen.sys, not drawing panic img...\r\n");
		goto doneImg;
	}

	uint16_t width  = filePtr[0];
	uint16_t height = filePtr[1];
	printf("width: %u\r\nheight: %u\r\n", width, height);
	for (uint16_t x = 0; x != width; x++) {
		for (uint16_t y = 0; y != height; y++) {
			size_t fbOff = (((y + ((bootboot.fb_height / 2) - (height / 2))) * bootboot.fb_width) +  (x + ((bootboot.fb_width / 2) - (width / 2))));
			#pragma GCC diagnostic push
			#pragma GCC diagnostic ignored "-Warray-bounds"
			((uint32_t *)&fb)[fbOff] = filePtr[(y * filePtr[0]) + x];
			#pragma GCC diagnostic pop
		}
	}
doneImg:
	serial.writeString(SERIAL_PORT_COM1, "done\r\n");
	serial.writeString(SERIAL_PORT_COM1, "halting...\r\n");
	while (true) {
		asm volatile (
			"cli\n"
			"hlt"
		);
	}
}
