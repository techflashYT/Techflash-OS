.global inb
.type inb,@function
/*
	%edi: address
*/
inb:
	mov %edi, %edx
	inb %dx, %al