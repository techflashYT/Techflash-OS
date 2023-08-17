#include <kernel/panic.h>
#include <kernel/arch.h>
#include <stdio.h>
void __attribute__((noreturn)) panic(char *message, registers_t *regs) {
	printf(
		"\r\n"
		"\r\n"
		"\e[0m\e[1m\e[31m !!! !!! !!! KERNEL PANIC !!! !!! !!!\e[0m\r\n"
		"Error: %s\r\n"
		"Registers:\r\n"
		"\r\n", message
	);
	ARCH_PrintRegs(regs);
	while (true) {
		asm volatile("cli; hlt");
	}
}