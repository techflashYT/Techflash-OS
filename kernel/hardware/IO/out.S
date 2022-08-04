.global outb
.global outw
.global outl
.global outq
.type outb, @function
.type outw, @function
.type outl, @function
.type outq, @function
/*
	%esi: value
	%edi: address
*/
outb:
	mov %esi, %eax
	mov %edi, %edx
	outb %al, %dx
