.global GDTFlush
.type GDTFlush, @function
GDTFlush:
	lgdt (%rdi)
	movw $0x10, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs
	movw %ax, %ss
	retfq
