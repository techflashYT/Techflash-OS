.section .data
__serialBadPanicString:
	.ascii "Your serial port is either not working or your BIOS is buggy."
	.byte 0
.section .text

.global __serialInit
.type __serialInit, @function

__serialInit:
	# Disable interrupts
	mov $0x00, %al
	mov $0x3F9, %dx # Root serial port address + 1
	outb %al, %dx

	# Get ready to set baud rate divisor
	mov $0x3FB, %dx # Root serial port address + 3
	mov $0x80, %al
	outb %al, %dx

	# Set baud rate divisor to 3 (38400 baud) [low byte]
	mov $0x3F8, %dx # Root serial port address
	mov $0x03, %al
	outb %al, %dx

	# Set baud rate divisor to 3 (38400 baud) [high byte]
	mov $0x3F9, %dx # Root serial port address + 1
	mov $0x00, %al
	outb %al, %dx

	# Set mode to 8-bits, no parity, 1 stop bit
	mov $0x3FB, %dx # Root serial port address + 3
	mov $0x03, %al
	outb %al, %dx

	# Enable FIFO, clear them, 14-byte threshold
	mov $0x3FA, %dx # Root serial port address + 2
	mov $0xC7, %al
	outb %al, %dx

	# Enable IRQs, RTS/DSR set
	mov $0x3FC, %dx # Root serial port address + 4
	mov $0x0B, %al
	outb %al, %dx

	# Loopback mode for testing		
	# 0x3FC (stays the same)
	mov $0x1E, %al
	outb %al, %dx

	# Send 0xAE to test if it works.
	mov $0x3F8, %dx # Root serial port address
	mov $0xAE, %al
	outb %al, %dx

	inb %dx, %al
	cmp $0xAE, %al
	jne .bad

	# Now that we know it works, put it in normal mode.
	mov $0x3FC, %dx # Root serial port address + 4
	mov $0x0F, %al
	outb %al, %dx
	mov $0x00, %rax
	ret
	.bad:
		mov $0x01, %rax
		ret