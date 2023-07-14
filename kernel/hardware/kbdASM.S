#.global KBD_StartSetLED
#.type KBD_StartSetLED, @function
.global KBD_GetAll
.type KBD_GetAll, @function
#.global KBD_GetStatusOfLED
#.type KBD_GetStatusOfLED, @function
.global KBD_WaitForACK
.type KBD_WaitForACK, @function
.global KBD_GetLED
.type KBD_GetLED, @function


KBD_StartSetLED:
	mov $0x60, %dx
	mov $0xED, %al
	out %al, %dx
	call KBD_WaitForACK
	xor %rax, %rax
	ret

KBD_GetAll:
	call KBD_StartSetLED
	mov $0x60, %dx
	inb %dx, %al
	call KBD_WaitForACK
	ret

KBD_GetLED:
	xor %rax, %rax
	xor %rcx, %rcx
	xor %rdx, %rdx
	call KBD_GetAll
	xchgq %rdi, %rcx
	shr %rcx, %al
	and $0x01, %al
	ret

KBD_WaitForACK:
	mov $0x60, %dx
	1:
		inb %dx, %al
		cmp $0xFA, %al
		jne 1b
	mov $0x00, %rax
	ret