#include <kernel/hardware/CPU/regs.h>
#include <kernel/hardware/CPU/ISR.h>
#include <kernel/hardware/IO.h>
#include <stdio.h>
#include <stdlib.h>
#include <kernel/misc.h>
#include <kernel/panic.h>
#include <kernel/tty.h>
isr_t interruptHandlers[256];
void ISRHandlersInit() {
	for (int i = 0; i < 256; i++) {
		interruptHandlers[i] = 0;
	}
}
void ISRHandler(registers_t* regs) {
	regs->intNo += 128;
	if (interruptHandlers[regs->intNo] != 0) {
		isr_t handler = interruptHandlers[regs->intNo];
		handler(regs);
	}
	panic("Unhandled Interrupt", *regs);
}
void registerInterruptHandler(uint8_t n, isr_t handler) {
	interruptHandlers[n] = handler;
}

/*void ISR1() {
	asm volatile (
		"cli\r\n"
		"push %rax\r\n"
		"push %rcx\r\n"
		"push %rdx\r\n"
		"push %r8\r\n"
		"push %r9\r\n"
		"push %r10\r\n"
		"push %r11\r\n"

		"mov %ds, %ax\r\n"
		"push %rax\r\n"
		"mov $0x10, %ax\r\n"
		"mov %ax, %ds\r\n"
		"mov %ax, %es\r\n"
		"mov %ax, %fs\r\n"
		"mov %ax, %gs\r\n"
	);
	outb(0x20, 0x20);
	outb(0xA0, 0x20);
	int status = inb(0x64);
	puts("\r\nISR1 occurred!  Status Read: 0x");
	char *buffer ="\0\0\0\0";
	itoa(status, buffer, 16);
	puts(buffer);
	puts("   Char Read: 0x");
	itoa(inb(0x60), buffer, 16);
	puts(buffer);
	puts("\r\n");

	asm volatile (
		"pop %rax\r\n"
		"mov %ax, %ds\r\n"
		"mov %ax, %es\r\n"
		"mov %ax, %fs\r\n"
		"mov %ax, %gs\r\n"

		"pop %r11\r\n"
		"pop %r10\r\n"
		"pop %r9\r\n"
		"pop %r8\r\n"
		"pop %rdx\r\n"
		"pop %rcx\r\n"
		"pop %rax\r\n"

		"iretq\r\n"
	);
}*/