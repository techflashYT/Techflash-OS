#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <kernel/hardware/serial.h>
void log(const char *module, const char* message) {
	char *buffer = malloc(256);

	buffer[0] = '[';
	memset(buffer + 1, ' ', 8);
	strcpy(buffer + 1, module);
	buffer[strlen(module) + 1] = ' ';
	buffer[9] = ']';
	buffer[10] = ' ';

	strcpy(buffer + 11, message);

	size_t end = strlen(buffer);

	buffer[end] = '\r';
	buffer[end + 1] = '\n';
	buffer[end + 2] = '\0';
	
	// Results in:
	// [{module}] {message}{newline}{NULL terminator}
	// neatly put into the buffer.
	serial.writeString(SERIAL_PORT_COM1, buffer);
	/* TODO: When logLevel kernel param is implemented, use the below code:
	if (env.logLevel < LOGLEVEL_WARN) {
		puts(buffer);
	}
	*/
	puts(buffer);
	free(buffer);
}