#include <stdio.h>
#include <stdint.h>
#include <kernel/hardware/serial.h>
#include <kernel/hardware/IO.h>
// MODULE("SERIAL");
void serialWriteString(const uint_fast16_t port, const char* value) {
	if (!serial.working) {
		return;
	}
	while (*value != '\0') {
		serial.write(port, *value);
		value++;
	}
}

uint_fast8_t serialReadBufEmpty(const uint_fast16_t port) {
	return inb(port + 5) & 1;
}

uint_fast8_t serialWriteBufEmpty(const uint_fast16_t port) {
	return inb(port + 5) & 0x20;
}
uint_fast8_t serialReadNext(const uint_fast16_t port) {
	if (!serial.working) {
		return 0;
	}
	while (serialReadBufEmpty(port) == 0);

	return inb(port);
}
 
void serialWrite(const uint_fast16_t port, const uint_fast8_t value) {
	if (!serial.working) {
		return;
	}
	// just append it to the buffer, it's fine as long as we don't run out (in which case oh crap we need to slow down)
	// while (serialWriteBufEmpty(port) == 0);

	outb(port, value);
}