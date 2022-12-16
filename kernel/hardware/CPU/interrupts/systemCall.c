#include <kernel/hardware/CPU/ISR.h>
#include <kernel/panic.h>
#include <kernel/misc.h>
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
void (*syscallHandlers[0xFFFF])(uint64_t arg1, uint64_t arg2, uint64_t arg3);
void initSyscalls() {
	syscallHandlers[0] = &putchar;
}
void systemCallHandler(registers_t *regsArg) {
	DUMPREGS
	uint64_t systemCallNumber = regsArg->rax;
	uint64_t args[3] = {
		regsArg->rdi,
		regsArg->rsi,
		regsArg->r8
	};
	if (syscallHandlers[systemCallNumber] == 0) {
		char *buffer = malloc(16);
		itoa(systemCallNumber, buffer, 10);
		char *str = "Unknown syscall: AAAAAAAAAAAAAAA";
		strcpy(str + 17, buffer);
		panic(str, regsArg);
	}
	syscallHandlers[systemCallNumber](args[0], args[1], args[2]);
}