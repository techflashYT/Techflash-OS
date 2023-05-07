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
	// we actually need this here, it slows everything dow, but there's actually no way to tell
	// when we've filled the buffer, or how close we are.
	while (serialWriteBufEmpty(port) == 0);

	outb(port, value);
}