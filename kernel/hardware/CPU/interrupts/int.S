.section .data
__temporaryStackPointer:
	.space 1*8
.section .text
.global ISRCommonStub
.macro ISR_NOERR num
.global ISR\num
ISR\num:
	cli
	push 0
	push \num
	jmp ISRCommonStub
.endm

.macro ISR_ERR num
.global ISR\num
ISR\num:
	cli
	push \num
	jmp ISRCommonStub
.endm

ISR_NOERR 0
ISR_NOERR 1
ISR_NOERR 2
ISR_NOERR 3
ISR_NOERR 4
ISR_NOERR 5
ISR_NOERR 6
ISR_NOERR 7
ISR_ERR   8
ISR_NOERR 9
ISR_ERR   10
ISR_ERR   11
ISR_ERR   12
ISR_ERR   13
ISR_ERR   14
ISR_NOERR 15
ISR_NOERR 16
ISR_ERR   17
ISR_NOERR 18
ISR_NOERR 19
ISR_NOERR 20
ISR_ERR   21
ISR_NOERR 22
ISR_NOERR 23
ISR_NOERR 24
ISR_NOERR 25
ISR_NOERR 26
ISR_NOERR 27
ISR_NOERR 28
ISR_NOERR 29
ISR_NOERR 30
ISR_NOERR 31

ISRCommonStub:
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

	call ISRHandler

	pop %rax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	
	pop %rdi
	pop %rsi
	pop %rbp
	add $8,%rsp
	pop %rbx
	pop %rdx
	pop %rcx
	pop %rax
	
	add $8, %esp
	sti
	iretq