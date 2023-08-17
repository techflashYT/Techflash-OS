#include <kernel/arch/x86/registers.h>
#include <stdio.h>
void ARCH_PrintRegs(registers_t *regs) {
	// manually grab CR2, since it isn't in the registers_t struct
	uint64_t cr2;
	asm volatile("mov %%cr2, %0" : "=r"(cr2));

	printf(
		"  RAX: 0x%016lX  RBX: 0x%016lX  RCX:  0x%016lX\r\n"
		"  RDX: 0x%016lX  RDI: 0x%016lX  RSI:  0x%016lX\r\n"
		"  R8:  0x%016lX  R9:  0x%016lX  R10:  0x%016lX\r\n"
		"  R11: 0x%016lX  R12: 0x%016lX  R13:  0x%016lX\r\n"
		"  R14: 0x%016lX  R15: 0x%016lX\r\n"
		"  Int: 0x%016lX  err: 0x%016lX  CR2:  0x%016lX\r\n"
		"  RIP: 0x%016lX  RBP: 0x%016lX  RSP:  0x%016lX\r\n"
		"  CS:  0x%016lX  SS:  0x%016lX  rflg: 0x%016lX\r\n",
		regs->rax, regs->rbx, regs->rcx,
		regs->rdx, regs->rdi, regs->rsi,
		regs->r8,  regs->r9,  regs->r10,
		regs->r11, regs->r12, regs->r13,
		regs->r14, regs->r15,
		regs->intNo, regs->errCode, cr2,
		regs->rip, regs->rbp, regs->rsp,
		regs->cs, regs->ss, regs->rflags
	);
}