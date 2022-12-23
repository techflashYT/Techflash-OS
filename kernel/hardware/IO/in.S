.global inb
.global inw
.global inl
.type inb, @function
.type inw, @function
.type inl, @function
/*
	%rdi: address
*/
inb:
	xor %rax, %rax
	xor %rdx, %rdx
	mov %rdi, %rdx
	inb %dx,  %al
	ret
inw:
	xor %rax, %rax
	xor %rdx, %rdx
	mov %rdi, %rdx
	inw %dx,  %ax
	ret
inl:
	xor %rax, %rax
	xor %rdx, %rdx
	mov %rdi, %rdx
	inl %dx,  %eax
	ret
