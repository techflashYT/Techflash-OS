.global CPUReset
.type CPUReset, @function
CPUReset:
	.__waitForKBDControllerEmpty:
		inb $0x64, %al
		test $0b00000010, %al
		jne .__waitForKBDControllerEmpty

	mov $0xFE, %al
	out %al, $0x64
	xor %ax, %ax
	ret
