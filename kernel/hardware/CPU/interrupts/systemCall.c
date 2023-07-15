#include <kernel/hardware/CPU/x86Setup.h>
#include <kernel/panic.h>
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <kernel/hardware/serial.h>
extern uint64_t putcharWrapper(uint64_t arg1, uint64_t arg2, uint64_t arg3);

void systemCallHandler(registers_t *regs);
uint64_t (*syscallHandlers[0xFF])(uint64_t arg1, uint64_t arg2, uint64_t arg3);
void initSyscalls() {
	registerInterruptHandler(0x80, &systemCallHandler);
	syscallHandlers[0] = &putcharWrapper;
}
void systemCallHandler(registers_t *regs) {
	uint64_t systemCallNumber = regs->rax;
	uint64_t args[3] = {
		regs->rdi,
		regs->rsi,
		regs->r8
	};
	if (syscallHandlers[systemCallNumber] == 0) {
		char str[32];
		sprintf(str, "Unknown syscall: %ld", systemCallNumber);
		panic(str, regs);
	}
	syscallHandlers[systemCallNumber](args[0], args[1], args[2]);
}
