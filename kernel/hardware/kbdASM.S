.global __keyboardStartSetLED
.type __keyboardStartSetLED, @function
.global __keyboardGetAll
.type __keyboardGetAll, @function
.global __keyboardGetStatusOfLED
.type __keyboardGetStatusOfLED, @function
.global __keyboardWaitForACK
.type __keyboardWaitForACK, @function


__keyboardStartSetLED:
	mov $0x60, %dx
	mov $0xED, %al
	out %al, %dx
	call __keyboardWaitForACK
	mov $0x00, %rax
	ret

__keyboardGetAll:
	call __keyboardStartSetLED
	mov $0x60, %dx
	inb %dx, %al
	call __keyboardWaitForACK
	ret

__keyboardGetStatusOfLED:
	call __keyboardGetAll
	xchgl %edi, %ecx
	shr %ecx, %al
	and $0x01, %al
	ret

__keyboardWaitForACK:
	mov $0x60, %dx
	.kbdACKLoop:
		inb %dx, %al
		cmp $0xFA, %al
		jne .kbdACKLoop
	mov $0x00, %rax
	ret