#include <stdio.h>
#include <stdint.h>
#include <kernel/hardware/serial.h>
#include <kernel/tty.h>
#include <kernel/graphics.h>
#include <kernel/hardware/IO.h>
#include <kernel/misc.h>
void __serialWriteString(const uint16_t port, const char* value) {
	if (!serial.working) {
		return;
	}
	while(*value != '\0') {
		serial.write(port, *value);
		value++;
	}
}
bool __serialInit() {
	outb(SERIAL_PORT_COM1 + 1, 0x00);    // Disable all interrupts
	ioWait();
	outb(SERIAL_PORT_COM1 + 3, 0x80);    // Enable DLAB (set baud rate divisor)
	ioWait();
	outb(SERIAL_PORT_COM1 + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
	ioWait();
	outb(SERIAL_PORT_COM1 + 1, 0x00);    //                  (hi byte)
	ioWait();
	outb(SERIAL_PORT_COM1 + 3, 0x03);    // 8 bits, no parity, one stop bit
	ioWait();
	outb(SERIAL_PORT_COM1 + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
	ioWait();
	outb(SERIAL_PORT_COM1 + 4, 0x0B);    // IRQs enabled, RTS/DSR set
	ioWait();
	outb(SERIAL_PORT_COM1 + 4, 0x1E);    // Set in loopback mode, test the serial chip
	ioWait();
	outb(SERIAL_PORT_COM1 + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)
	ioWait();

	// Check if serial is faulty (i.e: not same byte as sent)
	if (inb(SERIAL_PORT_COM1 + 0) != 0xAE) {
		serial.working = false;
		kernTTY.color = vga.colors.yellow;
		puts("WARNING");
		kernTTY.color = vga.colors.lgray;
		puts(": Your serial port appears not to be functioning, serial logging will be disabled.\r\n");
		return false;
	}
	serial.working = true;
	// If serial is not faulty set it in normal operation mode
	// (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
	outb(SERIAL_PORT_COM1 + 4, 0x0F);

	__serialWriteString(SERIAL_PORT_COM1, "SERIAL TEST!!!!!!!");
	return true;
}
int __serialReceived() {
	return inb(SERIAL_PORT_COM1 + 5) & 1;
}
char __serialReadNext() {
	if (!serial.working) {
		return 0;
	}
	while (__serialReceived() == 0);

	return inb(SERIAL_PORT_COM1);
}

int isEmpty() {
	return inb(SERIAL_PORT_COM1 + 5) & 0x20;
}
 
void __serialWrite(uint16_t port, uint8_t value) {
	if (!serial.working) {
		return;
	}
	while (isEmpty() == 0);

	outb(port, value);
}