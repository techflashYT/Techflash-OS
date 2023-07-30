#include <kernel/arch/x86/registers.h>
#include <stdio.h>
void ARCH_PrintRegs(registers_t *regs) {
	// manually grab CR2, since it isn't in the registers_t struct
	uint64_t cr2;
	asm volatile("mov %%cr2, %0" : "=r"(cr2));

	printf(
		"  RAX:   %016lX  RBX:   %016lX  RCX:   %016lX\r\n"
		"  RDX:   %016lX  RDI:   %016lX  RSI:   %016lX\r\n"
		"  R8:    %016lX  R9:    %016lX  R10:   %016lX\r\n"
		"  R11:   %016lX  R12:   %016lX  R13:   %016lX\r\n"
		"  R14:   %016lX  R15:   %016lX\r\n"
		"  Int:   %016lX  err:   %016lX  CR2:   %016lX\r\n"
		"  RIP:   %016lX  RBP:   %016lX  RSP:   %016lX\r\n"
		"  CS:    %016lX  SS:    %016lX  flags: %016lX\r\n",
		regs->rax, regs->rbx, regs->rcx,
		regs->rdx, regs->rdi, regs->rsi,
		regs->r8,  regs->r9,  regs->r10,
		regs->r11, regs->r12, regs->r13,
		regs->r14, regs->r15,
		regs->intNo, regs->errCode, cr2,
		regs->rip, regs->rbp, regs->userRsp,
		regs->cs, regs->ss, regs->rflags
	);
}