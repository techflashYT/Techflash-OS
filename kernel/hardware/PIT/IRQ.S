__IRQ0Handler:
	push %rbx

	mov (__IRQ0Fractions), %eax
	mov (__IRQ0MS), %ebx
	add %eax, (__systemTimerFractions)
	adc %ebx, (__systemTimerMS)

	mov $0x20, %al
	out %al, $0x20

	pop %rbx
	mov $0x00, %rax
	ret
