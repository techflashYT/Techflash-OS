.section .bss
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
	mov $kernelStackTop, %rsp
	# disable serial port interrupts since I know they cause issues
	movw $0xDEAD, (kernelStackTop)
	push %rax
	push %rdx
	mov $0x00, %rdx
	mov $0x3F9, %rax
	outb %al, %dx
	pop %rdx
	pop %rax
	call kernelMain
	# just in case
	.loop:
		cli
		hlt
		jmp .loop
