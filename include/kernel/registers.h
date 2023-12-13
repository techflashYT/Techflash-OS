#ifdef __x86_64__
#include <kernel/arch/x86/registers.h>
#endif

static inline void DUMPREGS(registers_t *regs) {
	asm ("mov %%rax,   %0" : "=r" (regs->rax));
	asm ("mov %%rbx,   %0" : "=r" (regs->rbx));
	asm ("mov %%rcx,   %0" : "=r" (regs->rcx));
	asm ("mov %%rdx,   %0" : "=r" (regs->rdx));
	asm ("mov %%rdi,   %0" : "=r" (regs->rdi));
	asm ("mov %%rsi,   %0" : "=r" (regs->rsi));
	asm ("mov %%rbp,   %0" : "=r" (regs->rbp));
	asm ("mov %%rsp,   %0" : "=r" (regs->rsp));
	asm ("mov %%r8,    %0" : "=r" (regs->r8));
	asm ("mov %%r9,    %0" : "=r" (regs->r9));
	asm ("mov %%r10,   %0" : "=r" (regs->r10));
	asm ("mov %%r11,   %0" : "=r" (regs->r11));
	asm ("mov %%r12,   %0" : "=r" (regs->r12));
	asm ("mov %%r13,   %0" : "=r" (regs->r13));
	asm ("mov %%r14,   %0" : "=r" (regs->r14));
	asm ("mov %%r15,   %0" : "=r" (regs->r15));
	asm ("mov %%cs,    %0" : "=r" (regs->cs));
	asm ("mov %%ss,    %0" : "=r" (regs->ss));
	asm ("sub $8, %%rsp; pushfq; popq %0; add $8, %%rsp" : "=r" (regs->rflags));
	asm ("lea (%%rip), %0" : "=r" (regs->rip));
	regs->intNo = 0x0;
	regs->errCode = 0x0;
}
