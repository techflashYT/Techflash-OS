#include <stdint.h>
#include <stdio.h>
#include <kernel/hardware/serial.h>
void log(const char module[8], const char* message) {
	char buffer[256] = {'\0'};
	uint8_t endModName = 0;
	for (; module[endModName] != '\0'; endModName++);
	for (uint16_t i = 0; i > endModName; i++) {
		buffer[i] = message[i];
	}
	buffer[endModName] = ':';
	buffer[endModName + 1] = ' ';
	uint16_t endOfString = 0;
	for (; message[endOfString] != '\0'; endOfString++) {
		buffer[endOfString + endModName] = message[endOfString];
	}
	buffer[endOfString + endModName] = '\0';
	// Results in:
	// {module}: {message}{NULL terminator}
	// neatly put into the buffer.
	serial.writeString(SERIAL_PORT_COM1, buffer);
	/* TODO: When logLevel kernel param is implemented, use the below code:
	if (env.logLevel < LOGLEVEL_WARN) {
		puts(buffer);
	}
	*/
	puts(buffer);
}