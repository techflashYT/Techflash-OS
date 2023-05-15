#include <kernel/hardware/CPU/x86Setup.h>
#include <stdlib.h>
#include <stdio.h>
char *smashedStr  = "Stack smashing detected (this stack trace might be bogus!)";


#if __LP64__
uint64_t __stack_chk_guard = 0x595e9fbd94fda766;
#else
uint32_t __stack_chk_guard = 0xe2dee396;
#endif

uint64_t *stackTrace(uint8_t max) {
	DUMPREGS;
	uint64_t *mem = malloc((max + 1) * 8);
	uint64_t *rbp = (uint64_t *)regs->rbp;
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