.global outb
.global outw
.global outl
.type outb, @function
.type outw, @function
.type outl, @function
/*
	%rsi: value
	%rdi: addrrss
*/
outb:
	xor  %rax, %rax
	mov  %rsi, %rax # Port
	mov  %rdi, %rdx # Data
	outb %al, %dx   # do the io
	ret
outw:
	xor  %rax, %rax
	mov  %rsi, %rax # Port
	mov  %rdi, %rdx # Data
	outw %ax, %dx   # do the io
	ret
outl:
	xor  %rax, %rax
	mov  %rsi, %rax # Port
	mov  %rdi, %rdx # Data
	outl %eax, %dx  # do the io
	ret
