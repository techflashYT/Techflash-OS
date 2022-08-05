#include <kernel/hardware/CPU/regs.h>
#include <kernel/hardware/CPU/ISR.h>
#include <kernel/hardware/IO.h>
#include <stdio.h>
isr_t interruptHandlers[256];
void ISRHandler(registers_t regs) {
	printf("Interrupt: %d\r\n", regs.intNo);
}
void registerInterruptHandler(uint8_t n, isr_t handler) {
	interruptHandlers[n] = handler;
}

void ISR1() {
	asm volatile (
		"xchgw %bx, %bx\r\n"
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
	printf("\r\nISR1 occurred!  Status Read: 0x%x", status);
	printf("   Char Read: 0x%x\r\n", inb(0x60));

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
}