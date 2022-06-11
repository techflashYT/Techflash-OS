.global _disableCursor
.type _disableCursor, @function
_disableCursor:
	pushf
	push %eax
	push %edx

	mov $0x3d4, %dx
	mov $0xa, %al
	out %al, %dx

	inc %dx
	mov $0x20, %al
	out %al, %dx

	pop %edx
	pop %eax
	popf
	ret
