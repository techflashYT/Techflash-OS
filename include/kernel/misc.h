#include <stdint.h>
#include <kernel/hardware/CPU/regs.h>

#define BREAK \
asm volatile (\
	"xchgw %bx, %bx\n"\
);

#define DUMPREGS \
_Pragma("GCC diagnostic push")\
_Pragma("GCC diagnostic ignored \"-Wuninitialized\"")\
register uint64_t rax asm("rax");\
register uint64_t rbx asm("rbx");\
register uint64_t rcx asm("rcx");\
register uint64_t rdx asm("rdx");\
register uint64_t rbp asm("rbp");\
register uint64_t rsp asm("rsp");\
register uint64_t rsi asm("rsi");\
register uint64_t rdi asm("rdi");\
registers_t regsTemp = {\
	.rax = rax,\
	.rbx = rbx,\
	.rcx = rcx,\
	.rdx = rdx,\
	.rbp = rbp,\
	.userRsp = rsp,\
	.rsi = rsi,\
	.rdi = rdi \
};\
registers_t *regs = &regsTemp;
_Pragma("GCC diagnostic pop")\
