.global _start
.type _start,@function

_start:
	# NULL out %rbp so that the stack works
	xor %rbp, %rbp

	# we need to do this crap for PIE (Position Independant Executables)
	lea x86_COM_Init(%rip), %rax
	call *%rax

	// lea x86_SSE_Init(%rip), %rax
	// call *%rax

	lea main(%rip), %rax
	call *%rax
.loop:
	lea .loop(%rip), %rax
	jmp *%rax
