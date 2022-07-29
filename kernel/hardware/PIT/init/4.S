initPIT.l4:
	movl %eax,__IRQ0Frequency          # Store the actual frequency for displaying later

	popq %rbx
	movl $0xDBB3A062,%eax
	mull %ebx
	shrdl $10,%edx,%eax
	shrl $10,%edx

	movl %edx,__IRQ0MS
	movl %eax,__IRQ0Fractions

	pushfq
	cli

	movb $0b00110100,%al
	outb %al, $0x43

	movw __PITReloadValue,%ax
	outb %al, $0x40
	movb %ah,%al
	outb %al, $0x40

	popfq

	pop %rdi
	pop %rsi
	pop %rbp
	add $8,%rsp
	pop %rbx
	pop %rdx
	pop %rcx
	pop %rax
	mov $0x00, %rax
	ret
