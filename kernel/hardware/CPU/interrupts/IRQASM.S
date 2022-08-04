.section .data
__temporaryStackPointer:
.space 1*8
.section .text
.global IRQHandler
.macro IRQ irqNum remapNum
.global IRQ\irqNum
IRQ\irqNum:
	cli
	push $0
	push $\remapNum
	jmp IRQCommonStub
.endm

IRQ   0,    32
IRQ   1,    33
IRQ   2,    34
IRQ   3,    35
IRQ   4,    36
IRQ   5,    37
IRQ   6,    38
IRQ   7,    39
IRQ   8,    40
IRQ   9,    41
IRQ  10,    42
IRQ  11,    43
IRQ  12,    44
IRQ  13,    45
IRQ  14,    46
IRQ  15,    47

IRQCommonStub:
	xchgw %bx, %bx
	xchgw %bx, %bx
	mov %esp, __temporaryStackPointer
	push %rax
	push %rcx
	push %rdx
	push %rbx
	push (__temporaryStackPointer)
	push %rbp
	push %rsi
	push %rdi

	mov %ds, %ax
	push %rax

	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs

	call IRQHandler

	pop %rbx
	mov %bx, %ds
	mov %bx, %es
	mov %bx, %fs
	mov %bx, %gs

	pop %rdi
	pop %rsi
	pop %rbp
	pop %rbx
	pop %rdx
	pop %rcx
	pop %rax
	mov __temporaryStackPointer, %esp

	add $8, %esp
	iretq