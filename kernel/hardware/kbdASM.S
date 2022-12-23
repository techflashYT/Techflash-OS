.global keyboardStartSetLED
.type keyboardStartSetLED, @function
.global keyboardGetAll
.type keyboardGetAll, @function
.global keyboardGetStatusOfLED
.type keyboardGetStatusOfLED, @function
.global keyboardWaitForACK
.type keyboardWaitForACK, @function


keyboardStartSetLED:
	mov $0x60, %dx
	mov $0xED, %al
	out %al, %dx
	call keyboardWaitForACK
	xor %rax, %rax
	ret

keyboardGetAll:
	call keyboardStartSetLED
	mov $0x60, %dx
	inb %dx, %al
	call keyboardWaitForACK
	ret

keyboardGetStatusOfLED:
	xor %rax, %rax
	xor %rcx, %rcx
	xor %rdx, %rdx
	call keyboardGetAll
	xchgq %rdi, %rcx
	shr %rcx, %al
	and $0x01, %al
	ret

keyboardWaitForACK:
	mov $0x60, %dx
	.kbdACKLoop:
		inb %dx, %al
		cmp $0xFA, %al
		jne .kbdACKLoop
	mov $0x00, %rax
	ret