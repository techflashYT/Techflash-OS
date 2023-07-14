#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <kernel/hardware/serial.h>
#include <kernel/bda.h>
#include <kernel/hardware/IO.h>
#include <kernel/hardware/CPU/x86Setup.h>
#include <kernel/panic.h>
#include <kernel/custom.h>
// MODULE("SERIAL");
serial_t serial;
void         serialHandler      (registers_t *regs);
uint_fast8_t serialReadNext     (const uint_fast16_t port);
void         serialWrite        (const uint_fast16_t port, const uint_fast8_t value);
void         serialWriteString  (const uint_fast16_t port, const char* value);
int          serialReadBufEmpty (const uint_fast16_t port);
int          serialWriteBufEmpty(const uint_fast16_t port);

uint16_t SERIAL_PORT_COM1;
uint16_t SERIAL_PORT_COM2;
uint16_t SERIAL_PORT_COM3;
uint16_t SERIAL_PORT_COM4;
// false = set up the struct and return
// true  = set up the serial port and return
bool structReady = false;
bool serialInit(const uint_fast64_t speed) {
	structReady          = true;
	serial.working       = false;
	serial.echo          = true;
	SERIAL_PORT_COM1     = bda->serialPortAddrs[0];
	SERIAL_PORT_COM2     = bda->serialPortAddrs[1];
	SERIAL_PORT_COM3     = bda->serialPortAddrs[2];
	SERIAL_PORT_COM4     = bda->serialPortAddrs[3];
	serial.init          = serialInit;
	serial.readNext      = serialReadNext;
	serial.write         = serialWrite;
	serial.writeString   = serialWriteString;
	serial.readBufEmpty  = serialReadBufEmpty;
	serial.writeBufEmpty = serialWriteBufEmpty;
	
	/*uint_fast8_t offset1 = 53;
	uint_fast8_t offset2 = 68;
	uint_fast8_t offset3 = 83;
	uint_fast8_t offset4 = 98;
	char *str = "I/O Ports according to the BIOS Data Area: Port 1: 0xAAA; Port 2: 0xAAA; Port 3: 0xAAA; Port 4: 0xAAA";
	char buffer[8];
	padTo(utoa(SERIAL_PORT_COM1, buffer, 16), 3);
	memcpy(str + offset1, buffer, 3);
	padTo(utoa(SERIAL_PORT_COM2, buffer, 16), 3);
	memcpy(str + offset2, buffer, 3);
	padTo(utoa(SERIAL_PORT_COM3, buffer, 16), 3);
	memcpy(str + offset3, buffer, 3);
	padTo(utoa(SERIAL_PORT_COM4, buffer, 16), 3);
	memcpy(str + offset4, buffer, 3);
	log(MODNAME, str, LOGLEVEL_DEBUG);*/
	// wait! wait! wait!
	// before we do anything, does the BIOS say that serial is disabled?
	uint8_t origPort = 0;
	origPort = SERIAL_PORT_COM1;
	if (SERIAL_PORT_COM1 == 0) {
		// it DOES!  Don't bother to do anything then.
		serial.working = false;

		// log(MODNAME, "Serial appears to be disabled in your computer's BIOS, testing to make sure.", LOGLEVEL_WARN);
		SERIAL_PORT_COM1 = 0x3F8; // Most common
	}
	outb(SERIAL_PORT_COM1 + 1, 0x00); ioWait();    // Disable all interrupts  (we already did this during boot, but just in case)
	outb(SERIAL_PORT_COM1 + 3, 0x80); ioWait();    // Enable DLAB (set baud rate divisor)
	if ((115200 % speed) != 0) {
		DUMPREGS
		panic("Tried to initialize serial with speed that isn't divisible by 115200!", regs);
	}
	uint16_t divFull = (115200 / speed);
	uint8_t  divLow  = ((uint8_t *)&divFull)[0];
	uint8_t  divHigh = ((uint8_t *)&divFull)[1];

	outb(SERIAL_PORT_COM1 + 0, divLow); ioWait();  // Set divisor (lo byte) to set baud rate.
	outb(SERIAL_PORT_COM1 + 1, divHigh); ioWait(); //             (hi byte)
	outb(SERIAL_PORT_COM1 + 3, 0x03); ioWait();    // 8 bits, no parity, one stop bit
	outb(SERIAL_PORT_COM1 + 2, 0xC7); ioWait();    // Enable FIFO, clear them, with 14-byte threshold
	outb(SERIAL_PORT_COM1 + 4, 0x0B); ioWait();    // IRQs enabled, RTS/DSR set
	outb(SERIAL_PORT_COM1 + 4, 0x1E); ioWait();    // Set in loopback mode, test the serial chip
	outb(SERIAL_PORT_COM1 + 0, 0xAE); ioWait();    // Test serial chip (send byte 0xAE and check if serial returns same byte)

	// registerInterruptHandler(0x24, &serialHandler); // register the interrupt handler for ports 1 & 3 on IRQ4
	// registerInterruptHandler(0x23, &serialHandler); // register the interrupt handler for ports 2 & 4 pm IRQ3
	// Check if serial is faulty (i.e: not same byte as sent)
	if (inb(SERIAL_PORT_COM1 + 0) != 0xAE) {
		serial.working = false;
		if (origPort == 0) {
			// log(MODNAME, "After testing, it appears that the serial port is indeed disabled.  Disabling serial logging...", LOGLEVEL_ERROR);
			return false;
		}
		// log(MODNAME, "Your serial port appears not to be functioning, serial logging will be disabled.", LOGLEVEL_ERROR);
		return false;
	}
	if (origPort == 0) {
		// log(MODNAME, "After testing, it appears that the serial port is actually working despite the BIOS saying it isn't!  Enabling serial logging...", LOGLEVEL_INFO);
	}
	// If serial is not faulty set it in normal operation mode
	// (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
	serial.working = true; // remember that serial is working
	outb(SERIAL_PORT_COM1 + 4, 0x0F);
	// enable interrupts
	outb(SERIAL_PORT_COM1 + 1, 0x01);

	serialWriteString(SERIAL_PORT_COM1, "SERIAL TEST!!!!!!!\r\n");
	return true;
}
