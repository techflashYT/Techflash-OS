.global isr1
.type isr1, @function
.global loadIDT
.type loadIDT, @function

.global isr1Handler

.global _idt
idtDescriptor:
	.short 0xFFF
	.short $_idt

.macro PUSHALL
	push %rax
	push %rcx
	push %rdx
	push %r8
	push %r9
	push %r10
	push %r11
.endm

.macro POPALL
	pop %r11
	pop %r10
	pop %r9
	pop %r8
	pop %rdx
	pop %rcx
	pop %rax
.endm
isr1:
	PUSHALL
	call isr1Handler
	POPALL
	iretq


loadIDT:
	lidt $idtDescriptor
	sti
	ret
