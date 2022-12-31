#include <stdint.h>
#include <kernel/panic.h>
#include <kernel/misc.h>
#if UINT32_MAX == UINTPTR_MAX
#define STACK_CHK_GUARD 0xe2dee396
#else
#define STACK_CHK_GUARD 0x595e9fbd94fda766
#endif
const char *panicStr = "Kernel Stack Overflow";
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;
__attribute__((noreturn)) void __stack_chk_fail(void) {
	DUMPREGS
	// asm because I don't trust the compiler to not mess with the stack here
	asm volatile (
		"mov kernelStackTop, %rax;"
		"cmp $0xAD, 1(%rax);"
		"jne .stackOverflow;"
		"cmp $0xDE, 0(%rax);"
		"jne .stackOverflow;"
		"jmp .bye;"
		".stackOverflow:;"
		"mov panicStr, %rdi;"
		"mov regsDump, %rsi;"
		"call panic;"
		".bye:;"
	);
	panic("Stack smashing detected", regs);
	while (1);
}