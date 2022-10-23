#include <stdint.h>
#include <kernel/hardware/serial.h>
#include <kernel/panic.h>
#include <kernel/hardware/IO.h>
#include <kernel/misc.h>
void __serialWriteString(const uint16_t port, const char* value) {
	while(*value != '\0') {
		serial.write(port, *value);
		value++;
	}
}
void __serialInit() {
	outb(SERIAL_PORT_COM1 + 1, 0x00);    // Disable all interrupts
	outb(SERIAL_PORT_COM1 + 3, 0x80);    // Enable DLAB (set baud rate divisor)
	outb(SERIAL_PORT_COM1 + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
	outb(SERIAL_PORT_COM1 + 1, 0x00);    //                  (hi byte)
	outb(SERIAL_PORT_COM1 + 3, 0x03);    // 8 bits, no parity, one stop bit
	outb(SERIAL_PORT_COM1 + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
	outb(SERIAL_PORT_COM1 + 4, 0x0B);    // IRQs enabled, RTS/DSR set
	outb(SERIAL_PORT_COM1 + 4, 0x1E);    // Set in loopback mode, test the serial chip
	outb(SERIAL_PORT_COM1 + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)

	// Check if serial is faulty (i.e: not same byte as sent)
	if (inb(SERIAL_PORT_COM1 + 0) != 0xAE) {
		DUMPREGS
		panic("bad serial", regs);
	}

	// If serial is not faulty set it in normal operation mode
	// (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
	outb(SERIAL_PORT_COM1 + 4, 0x0F);

	__serialWriteString(SERIAL_PORT_COM1, "SERIAL TEST!!!!!!!");
	return 0;
}
int __serialRecieved() {
	return inb(SERIAL_PORT_COM1 + 5) & 1;
}
char __serialReadNext() {
	while (__serialRecieved() == 0);

	return inb(SERIAL_PORT_COM1);
}

int isEmpty() {
	return inb(SERIAL_PORT_COM1 + 5) & 0x20;
}
 
void __serialWrite(uint16_t port, uint8_t value) {
	while (isEmpty() == 0);

	outb(SERIAL_PORT_COM1, value);
}