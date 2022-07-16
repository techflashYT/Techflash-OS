#include <kernel/io.h>
#include <kernel/hardware/serial.h>

int __serialInit() {
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
   if(inb(SERIAL_PORT_COM1 + 0) != 0xAE) {
      return 1;
   }

   // If serial is not faulty set it in normal operation mode
   // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
   outb(SERIAL_PORT_COM1 + 4, 0x0F);
   return 0;
}
// Private function to check if there is a character in the serial buffer
int __serialRecieved(uint16_t port) {
	return inb(port + 5) & 1;
}
// Read next character from serial buffer
char __serialReadNext(uint16_t port) {
	while (__serialRecieved(SERIAL_PORT_COM1) == 0); // Wait for a character to be recieved (FIXME: This is BAD, it keeps the CPU busy waiting while waiting for anything on the serial buffer.)
	return inb(port);
}
// Private function to check if the transmit buffer is empty
int __serialTransmitEmpty(uint16_t port) {
	return inb(port + 5) & 0x20;
}
// Write a character to the serial buffer
void __serialWrite(uint16_t port, uint8_t value) {
	while (__serialTransmitEmpty(SERIAL_PORT_COM1) == 0); // Wait for the transmit buffer to be empty
	outb(port, value);
}