.global outb
.type outb, @function
.global outw
.type outw, @function
.global outl
.type outl, @function

outb:
	pop %dx
	outb %al, %dx
	ret

outw:
	pop %dx
	outw %ax, %dx
	ret

outl:
	pop %rdx
	mov $0x0, %rax
	outl %eax, %edx
	ret
