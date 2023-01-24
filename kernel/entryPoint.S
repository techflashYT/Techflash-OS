.section .bss
.global emergencyStack
emergencyStack:
.quad 0x0000000000000000 # space for pointer

.global kernelStackBottom
.global kernelStackTop
kernelStackBottom:
.skip 0x150001, 0
kernelStackTop:
.word 0x0000 // (will be initialized to 0xDEAD in a sec) if this gets overwritten, we've stack overflowed
.section .text

.global kernelMain
.type kernelMain, @function
.global _start
.type _start, @function
_start:
	cli
	movq %rsp, emergencyStack  # Move bootboot's stack pointer into the emergencyStack variable, for use in case of a stack overflow
	mov  $kernelStackTop, %rsp # Move our custom stack into the stack pointer
	movw $0xDEAD, (kernelStackTop) # Move `0xDEAD` into the very end of the stack, if that gets overwritten, we're dead

	# disable serial port interrupts since I know they cause issues
	push %rax
	push %rdx
	mov  $0x00, %rdx
	mov  $0x3F9, %rax
	outb %al, %dx
	pop  %rdx
	pop  %rax

	# call the main kernel function in C
	call kernelMain
	
	# just in case kernelMain somehow returns
	.loop:
		cli
		hlt
		jmp .loop
