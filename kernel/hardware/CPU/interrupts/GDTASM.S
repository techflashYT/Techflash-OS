.global GDTFlush
.type GDTFlush, @function
GDTFlush:
	xchgw %bx, %bx
	movq 4(%rsp), %rax
	lgdt (%rax)

	movw $0x10, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs
	movw %ax, %ss
	jmp %cs:.flush
.flush:
	ret
