#include <stdlib.h>
#include <string.h>
#include <kernel/hardware/CPU/x86Setup.h>
#include <kernel/hardware/serial.h>
#include <stdio.h>
void serialHandler(registers_t *regs) {
	// we got something from serial, echo it
	// if (serial.writeBufEmpty(SERIAL_PORT_COM1) == 0) {
	// 	// empty, nothing to do, leave
	// 	return;
	// }
	(void)regs;
	uint_fast8_t byte = serial.readNext(SERIAL_PORT_COM1);
	if (serial.echo) {
		if (byte == '\r') {
			serial.write(SERIAL_PORT_COM1, '\n');
		}
		serial.write(SERIAL_PORT_COM1, byte);
	}
	
	return;
}