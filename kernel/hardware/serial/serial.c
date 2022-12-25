#include <stdio.h>
#include <stdint.h>
#include <kernel/hardware/serial.h>
#include <kernel/hardware/IO.h>
MODULE("SERIAL");
void serialWriteString(const uint16_t port, const char* value) {
	if (!serial.working) {
		return;
	}
	while (*value != '\0') {
		serial.write(port, *value);
		value++;
	}
}

int serialReadBufEmpty(const uint16_t port) {
	return inb(port + 5) & 1;
}

int serialWriteBufEmpty(const uint16_t port) {
	return inb(port + 5) & 0x20;
}
char serialReadNext(const uint16_t port) {
	if (!serial.working) {
		return 0;
	}
	while (serialReadBufEmpty(port) == 0);

	return inb(port);
}
 
void serialWrite(const uint16_t port, const uint8_t value) {
	if (!serial.working) {
		return;
	}
	// just append it to the buffer, it's fine as long as we don't run out (in which case oh crap we need to slow down)
	// while (serialWriteBufEmpty(port) == 0);

	outb(port, value);
}