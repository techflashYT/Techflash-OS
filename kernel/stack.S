overflowStr:
.asciz "Kernel Stack Overflow (this stack trace trace might be bogus!)"
smashedStr:
.asciz "Stack smashing detected (this stack trace might be bogus!)"
.global __stack_chk_guard
.global __stack_chk_fail
__stack_chk_guard:
#if defined(__X86_64__) || defined(_M_X64) || defined (__aarch64__) || defined(_M_ARM64)
.quad 0x595e9fbd94fda766
#else
.long 0xe2dee396
#endif
backupRAX:
.quad 0
__stack_chk_fail:
	movq %rax, backupRAX
	movq regsDump, %rax
	
	movq %rdi,           0(%rax)
	movq %rsi,           8(%rax)
	movq %rbp,          16(%rax)
	movq %rbx,          24(%rax)
	movq %rdx,          32(%rax)
	movq %rcx,          40(%rax)
	movq $backupRAX,    48(%rax)
	leaq 0x0(%rip),        %rbx
	mov  %rbx,		   128(%rax)

	mov kernelStackTop, %rax
	cmpb $0xAD, 1(%rax)
	jne .stackOverflow
	cmpb $0xDE, 0(%rax)
	jne .stackOverflow
	jmp .bye
	.stackOverflow:
		mov (emergencyStack), %rsp
		mov overflowStr, %rdi
		mov regsDump, %rsi
		jmp panic
	.bye: // didn't overflow, but corrupted anyways
		mov smashedStr, %rdi
		mov regsDump, %rsi
		jmp panic

