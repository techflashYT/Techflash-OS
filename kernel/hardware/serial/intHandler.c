#include <stdlib.h>
#include <string.h>
#include <kernel/hardware/CPU/ISR.h>
#include <kernel/hardware/serial.h>
#include <kernel/log.h>
void serialHandler(registers_t *regs) {
	// serial clock!  let's check if we got anything
	if (serial.readBufEmpty(SERIAL_PORT_COM1) == 0) {
		// empty, nothing to do, leave
		return;
	}
	(void)regs;
	char *buffer = malloc(64);
	strcpy(buffer, "Received a byte!  Byte: 0x");
	utoa(serial.readNext(SERIAL_PORT_COM1), buffer + strlen(buffer), 16);
	log("SERIAL", buffer, LOGLEVEL_DEBUG);
	free(buffer);
	return;
}