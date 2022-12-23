#include <stdint.h>
#include <kernel/hardware/serial.h>
#include <kernel/log.h>
#include <kernel/hardware/IO.h>
#include <kernel/hardware/CPU/IRQNums.h>
#include <kernel/hardware/CPU/ISR.h>
#include <kernel/misc.h>
#include <kernel/panic.h>
void serialHandler(registers_t *regs);
char serialReadNext(const uint16_t port);
void serialWrite(const uint16_t port, const uint8_t value);
void serialWriteString(const uint16_t port, const char* value);
int serialReadBufEmpty(const uint16_t port);
int serialWriteBufEmpty(const uint16_t port);
// false = set up the struct and return
// true  = set up the serial port and return
bool structReady = false;
bool serialInit(const uint64_t speed) {
	// is the struct ready yet?  if not, this is the first time calling serialInit,
	// we're still in `initThings` initializing globals, so let's initialize the struct.
	if (!structReady) {
		structReady          = true;
		serial.init          = serialInit;
		serial.readNext      = serialReadNext;
		serial.write         = serialWrite;
		serial.writeString   = serialWriteString;
		serial.readBufEmpty  = serialReadBufEmpty;
		serial.writeBufEmpty = serialWriteBufEmpty;
		return false;
	}
	// it's true, lets enable serial
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

	registerInterruptHandler(IRQ4, &serialHandler); // register the interrupt handler for ports 1 & 3
	// Check if serial is faulty (i.e: not same byte as sent)
	if (inb(SERIAL_PORT_COM1 + 0) != 0xAE) {
		serial.working = false;
		log("SERIAL", "Your serial port appears not to be functioning, serial logging will be disabled.", LOGLEVEL_WARN);
		return false;
	}
	// If serial is not faulty set it in normal operation mode
	// (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
	serial.working = true; // remember that serial is working
	ioWait(); ioWait();
	// registerInterruptHandler(IRQ3, &serialHandler); // register the interrupt handler for ports 2 & 4
	outb(SERIAL_PORT_COM1 + 4, 0x0F);
	// enable interrupts
	outb(SERIAL_PORT_COM1 + 1, 0x02);

	serialWriteString(SERIAL_PORT_COM1, "SERIAL TEST!!!!!!!\r\n");
	return true;
}