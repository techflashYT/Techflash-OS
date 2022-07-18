.section .text

.global __serialReadNext
.type __serialReadNext, @function
.global __serialReadNext
.type __serialReadNext, @function
.global __serialWrite
.type __serialWrite, @function
.global __serialWriteString
.type __serialWriteString, @function
.global panic
.type panic, @function
__serialRecieved:
	pop %dx
	add $5, %dx
	inb %dx, %al
	and $1, %al;
// Read next character from serial buffer
__serialReadNext:
	mov %edx, %edi
	mov $0x3F8, %edi
	.readLoop:
		call __serialRecieved
		test %eax, %eax
		je .readLoop
	inb %dx, %al
	ret
// Private function to check if the transmit buffer is empty
__serialTransmitEmpty:
	add $5, %dx
	xchgl %edi, %edx
	inb %dx, %al
	and $0x20, %al
	ret
// Write a character to the serial buffer
__serialWrite:
	mov %edi,%edx // port
	mov %esi,%eax // value
	push %rax
	.writeLoop:
		call __serialTransmitEmpty
		test %eax, %eax
		je .writeLoop
	pop %rax
	outb %al, %dx
	xor %ax, %ax
	ret
