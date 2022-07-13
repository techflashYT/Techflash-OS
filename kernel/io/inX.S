.global inb
.type inb, @function
.global inw
.type inw, @function
.global inl
.type inl, @function

inb:
	pop %dx
	inb %dx, %al
	ret

inw:
	pop %dx
	inw %dx, %ax
	ret

inl:
	pop %rdx
	mov $0x0, %rax
	inl %edx, %eax
	ret
