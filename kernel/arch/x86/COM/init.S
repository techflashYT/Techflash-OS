.global SERIAL_PORT_COM1
.global SERIAL_PORT_COM2
.global SERIAL_PORT_COM3
.global SERIAL_PORT_COM4
.global x86_COM_Init
.global x86_COM_Working


#define COM1_IO 0x3F8

.section .data
SERIAL_PORT_COM1: .word COM1_IO
SERIAL_PORT_COM2: .word 0x2F8
SERIAL_PORT_COM3: .word 0x3E8
SERIAL_PORT_COM4: .word 0x2E8

.section .bss
# it starts as 0, so it can be in the BSS
x86_COM_Working:  .byte 0x0


.section .text
x86_COM_Init:
	mov $COM1_IO + 1, %dx
	xor %al, %al
	# outb(COM1_IO + 1, 0); - Disable serial interrupts
	outb %al, %dx


	add $2, %dx
	mov $0x80, %al
	# outb(COM1_IO + 3, 0x80); - Enable DLAB
	outb %al, %dx


	sub $3, %dx
	mov $0x01, %al
	# outb(COM1_IO + 0, 0x00); - Set low byte of divisor to 1 (115200 baud)
	outb %al, %dx


	inc %dx
	xor %al, %al
	# outb(COM1_IO + 1, 0x00); - Set high byte of divisor to 0
	outb %al, %dx


	add $2, %dx
	mov $0x03, %al
	# outb(COM1_IO + 3, 0x03); - 8n1 mode, the most common (8 bits data, 0 bits parity, 1 stop bit)
	outb %al, %dx


	dec %dx
	mov $0xC7, %al
	# outb(COM1_IO + 2, 0xC7); - Enable FIFO, cleared, with 14 byte threshold
	outb %al, %dx


	add $2, %dx
	mov $0x0B, %al
	# outb(COM1_IO + 4, 0x0B); - IRQs on(!!), RTS/DSR set
	outb %al, %dx


	mov $0x1E, %al
	# outb(COM1_IO + 4, 0x1E); - Loopback mode
	outb %al, %dx


	sub $4, %dx
	mov $0xAE, %al
	# outb(COM1_IO + 0, 0xAE); - Send byte 0xAE to test
	outb %al, %dx

	# read from serial port
	inb %dx, %al

	# is the byte that we got back 0xAE?
	cmp $0xAE, %al
	// lea .badSerial(%rip), %rbx
	// jne *%rbx
	jne .badSerial

	# YES!  We have working serial!
	add $4, %dx
	mov $0x0F, %al
	# outb(COM1_IO + 0, 0x0F); - IRQs on(!!), OUT#1 & OUT#2 enabled, no loopback
	outb %al, %dx

	# return success
	xor %al, %al
	ret

.badSerial:
	# yikes, no serial :'(
	// no need to save this info, because the default value is already 0 (false)
	mov $0x01, %al
	ret
