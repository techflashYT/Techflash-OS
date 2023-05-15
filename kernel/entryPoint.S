.section .text
.global kernelMain
.type kernelMain, @function
.global _start
.type _start, @function
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

	# call the main kernel function in C
	call kernelMain
	
	# just in case kernelMain somehow returns
	.loop:
		cli
		hlt
		jmp .loop
