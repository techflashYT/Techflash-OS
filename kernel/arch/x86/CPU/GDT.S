.global x86_GDT_Flush
x86_GDT_Flush:
	lgdt (%rdi)
	movw $0x10, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs
	movw %ax, %ss
	
	pushq $0x08   // push CS
	pushq 8(%rsp) // push return address
	lretq $0x08   // return there, clearing old address
