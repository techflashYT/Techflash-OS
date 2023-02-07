#include <stdint.h>
#include <kernel/hardware/CPU/x86Setup.h>

#define BREAK \
asm volatile (\
	"xchgw %bx, %bx\n"\
);

#define DUMPREGS \
extern registers_t regsDump;\
asm("movq %%rax, %0\r\n" : "=r" (regsDump.rax) : );\
asm("movq %%rbx, %0\r\n" : "=r" (regsDump.rbx) : );\
asm("movq %%rcx, %0\r\n" : "=r" (regsDump.rcx) : );\
asm("movq %%rdx, %0\r\n" : "=r" (regsDump.rdx) : );\
asm("movq %%rdi, %0\r\n" : "=r" (regsDump.rdi) : );\
asm("movq %%rsi, %0\r\n" : "=r" (regsDump.rsi) : );\
asm("movq %%rsp, %0\r\n" : "=r" (regsDump.userRsp) : );\
asm("movq %%rbp, %0\r\n" : "=r" (regsDump.rbp) : );\
asm("leaq 0x0(%%rip), %0\r\n" : "=r" (regsDump.rip) : );\
registers_t *regs = &regsDump;
