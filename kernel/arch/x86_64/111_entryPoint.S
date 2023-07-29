.global _start
.type _start,@function

_start:
	# NULL out %rbp so that the stack works
	xor %rbp, %rbp
	call x86_COM_Init
	call x86_SSE_Init
	call main

.loop:
	jmp .loop