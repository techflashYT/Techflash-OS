.global ioWait
.type ioWait, @function

ioWait:
	push %ax
	push %dx

	mov $0x0, %ax
	mov $0x80, %dx

	outb %al, %dx

	pop %dx
	pop %ax
	ret