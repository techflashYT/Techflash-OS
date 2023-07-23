.section .text
.global kernelMain
.type kernelMain, @function
.global _start
.type _start, @function

/*
Very early initialization.  Stuff that can't be done safely from within the C code.
*/

_start:
	cli

	# set RBP to NULL so that stack traces can just work
	xor %rbp, %rbp
	/*
	# disable serial port interrupts since I know they cause issues
	push %rax
	push %rdx
	mov  $0x00, %rdx
	mov  $0x3F9, %rax
	outb %al, %dx
	pop  %rdx
	pop  %rax
	*/

	# enable SSE, since the C code uses it.
	call SSEInit

	# 
	# call the main kernel function in C
	call kernelMain
	
	nop
	# nop here to add one extra byte so that the stack trace gets the right symbols

	# just in case kernelMain somehow returns
	.loop:
		cli
		hlt
		jmp .loop
