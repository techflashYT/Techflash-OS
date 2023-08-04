.global x86_ISR_StubArray

.section .text
.global ISRCommonStub
.macro PUSHALL
push %r15
push %r14
push %r13
push %r12
push %r11
push %r10
push %r9
push %r8
push %rax
push %rcx
push %rdx
push %rbx
push %rbp
push %rsi
push %rdi
.endm
.macro POPALL
pop %rdi
pop %rsi
pop %rbp
pop %rbx
pop %rdx
pop %rcx
pop %rax
pop %r8
pop %r9
pop %r10
pop %r11
pop %r12
pop %r13
pop %r14
pop %r15
.endm

.macro ISR_NOERR num
.align 8
   push $\num-128
   jmp x86_ISR_StubNoErr
.align 8
.endm

.macro ISR_ERR num
.align 8
   push $\num-128
   jmp x86_ISR_StubErr
.align 8
.endm

x86_ISR_StubArray:
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
ISR_ERR   29
ISR_ERR   30
ISR_NOERR 31

.set i,32
.rept 256-32
ISR_NOERR i
.set i,i+1
.endr

x86_ISR_StubNoErr:
	pushq (%rsp) # copy interrupt number to the correct location
x86_ISR_StubErr:
	cld
	PUSHALL
	mov %rsp, %rdi # Save start of struct as pointer in %rdi (first argument according to x86_64 SYSV ABI)
	mov %rsp, %r12
	andq $-16, %rsp # Align stack to 16 bytes
	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs

	call x86_ISR_Handler

	movq %r12, %rsp # Restore stack pointer
	mov $0x10, %rax # Segment selector for kernel code
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	
	call x86_PIC_SendEOI

	POPALL
	add $16, %rsp
	iretq