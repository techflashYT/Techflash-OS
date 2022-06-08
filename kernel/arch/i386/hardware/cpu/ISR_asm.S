	.global isrHandler
isrCommonStub:
	# Save regs
	pusha
	mov %ds, %ax
	push %eax
	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	
	# Call the Interrupt Service Routine Handler from C
	call isrHandler
	# Restore regs
	pop %eax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	popa
	add $0x8, %esp
	sti
	iret

	# Show these funcs to the rest of the code
	.global isr0
	.global isr1
	.global isr2
	.global isr3
	.global isr4
	.global isr5
	.global isr6
	.global isr7
	.global isr8
	.global isr9
	.global isr10
	.global isr11
	.global isr12
	.global isr13
	.global isr14
	.global isr15
	.global isr16
	.global isr17
	.global isr18
	.global isr19
	.global isr20
	.global isr21
	.global isr22
	.global isr23
	.global isr24
	.global isr25
	.global isr26
	.global isr27
	.global isr28
	.global isr29
	.global isr30
	.global isr31

# Divide by zero
isr0:
	cli
	push $0
	push $0
	jmp isrCommonStub
# Debug
isr1:
	cli
	push $0
	push $1
	jmp isrCommonStub
# NMI
isr2:
	cli
	push $0
	push $2
	jmp isrCommonStub
# Breakpoint
isr3:
	cli
	push $0
	push $3
	jmp isrCommonStub
# ?
isr4:
	cli
	push $0
	push $4
	jmp isrCommonStub
# Out of bounds
isr5:
	cli
	push $0
	push $5
	jmp isrCommonStub
# Invalid opcode
isr6:
	cli
	push $0
	push $6
	jmp isrCommonStub
# No coprocessor
isr7:
	cli
	push $0
	push $7
	jmp isrCommonStub
# Double fault (uh oh!)
isr8:
	cli
	push $8
	jmp isrCommonStub
# Coprocessor segment overrun
isr9:
	cli
	push $0
	push $9
	jmp isrCommonStub
# Bad TSS
isr10:
	cli
	push $10
	jmp isrCommonStub
# Segment not present
isr11:
	cli
	push $11
	jmp isrCommonStub
# Stack fault
isr12:
	cli
	push $12
	jmp isrCommonStub
# General protection fault
isr13:
	cli
	push $13
	jmp isrCommonStub
# Page fault
isr14:
	cli
	push $14
	jmp isrCommonStub
# Reserved Exception
isr15:
	cli
	push $0
	push $15
	jmp isrCommonStub
# Floating Point Exception
isr16:
	cli
	push $0
	push $16
	jmp isrCommonStub
# Alignment Check Exception
isr17:
	cli
	push $0
	push $17
	jmp isrCommonStub
# Machine Check Exception
isr18:
	cli
	push $0
	push $18
	jmp isrCommonStub
# Reserved
isr19:
	cli
	push $0
	push $19
	jmp isrCommonStub
# Reserved
isr20:
	cli
	push $0
	push $20
	jmp isrCommonStub
# Reserved
isr21:
	cli
	push $0
	push $21
	jmp isrCommonStub
# Reserved
isr22:
	cli
	push $0
	push $22
	jmp isrCommonStub
# Reserved
isr23:
	cli
	push $0
	push $23
	jmp isrCommonStub
# Reserved
isr24:
	cli
	push $0
	push $24
	jmp isrCommonStub
# Reserved
isr25:
	cli
	push $0
	push $25
	jmp isrCommonStub
# Reserved
isr26:
	cli
	push $0
	push $26
	jmp isrCommonStub
# Reserved
isr27:
	cli
	push $0
	push $27
	jmp isrCommonStub
# Reserved
isr28:
	cli
	push $0
	push $28
	jmp isrCommonStub
# Reserved
isr29:
	cli
	push $0
	push $29
	jmp isrCommonStub
# Reserved
isr30:
	cli
	push $0
	push $30
	jmp isrCommonStub
# Reserved
isr31:
	cli
	push $0
	push $31
	jmp isrCommonStub
