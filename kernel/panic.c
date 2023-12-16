#include <kernel/panic.h>
#include <kernel/registers.h>
#include <kernel/arch.h>
#include <stdio.h>
#include <stdbool.h>

uint64_t mem[20];
uint64_t *stackTrace(uint8_t max) {
    (void)max;
	registers_t regs;
    DUMPREGS(&regs);

    uint64_t *rbp = (uint64_t *)regs.rbp;
    uint8_t i = 0;
    while (rbp) {
        uint64_t rip = *(rbp + 1);
        rbp = (uint64_t *)*rbp;

        mem[i] = rip;
        i++;

        if (!rip) {
            break;
        }
    }
    mem[i] = 0;
    return mem;
}

	
void __attribute__((noreturn)) panic(char *message, registers_t *regs) {
	printf(
		"\r\n"
		"\r\n"
		"\e[0m\e[1m\e[31m !!! !!! !!! KERNEL PANIC !!! !!! !!!\e[0m\r\n"
		"Error: %s\r\n"
		"Techflash OS Version %s\r\n"
		"Registers:\r\n"
		"\r\n", message, VER_STR
	);
	
	ARCH_PrintRegs(regs);
	puts("Stack Trace:");

	uint64_t *trace = stackTrace(20);
	for (int i = 0; i != 20; i++) {
		if (trace[i] == 0) {
			break;
		}
		printf("  #%d: %p\r\n", i, (void *)trace[i]);
	}
	while (true) {
		asm volatile("cli; hlt");
	}
}