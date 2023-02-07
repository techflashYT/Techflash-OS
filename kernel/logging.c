#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <kernel/hardware/serial.h>
#include <kernel/graphics.h>
#include <kernel/tty/tty.h>
#define LOG_TO_SERIAL
#define LOG_COLOR
char *logLevelColorsEscCode[] = {
	"\x1b[0m", // verbose
	"\x1b[0m", // debug
	"\x1b[0m\x1b[1;39m", // info
	"\x1b[0m\x1b[33m", // warn
	"\x1b[0m\x1b[31m"  // err
};
uint32_t logLevelColorsFrameBuf[] = {
	0x00AAAAAA,   // verbose
	0x00AAAAAA,   // debug
	0x00DDDDDD,   // info
	warningColor, // warn
	errorColor    // err
};
colorChar_t *logBuffer;
void log(const char *module, const char* message, uint8_t logLevel) {
	if (logLevel < kernel.logLevel) {
		return;
	}
	// colorize using actual ANSI escape sequences for the serial logging
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
	#ifdef LOG_TO_SERIAL
		#ifdef LOG_COLOR
			serial.writeString(SERIAL_PORT_COM1, "\x1b[0m");
			serial.writeString(SERIAL_PORT_COM1, "\x1b[33m[\x1b[1;36m");
			serial.write(SERIAL_PORT_COM1, buffer[1]);
			serial.write(SERIAL_PORT_COM1, buffer[2]);
			serial.write(SERIAL_PORT_COM1, buffer[3]);
			serial.write(SERIAL_PORT_COM1, buffer[4]);
			serial.write(SERIAL_PORT_COM1, buffer[5]);
			serial.write(SERIAL_PORT_COM1, buffer[6]);
			serial.write(SERIAL_PORT_COM1, buffer[7]);
			serial.write(SERIAL_PORT_COM1, buffer[8]);
			serial.writeString(SERIAL_PORT_COM1, "\x1b[0m\x1b[33m]");
			serial.writeString(SERIAL_PORT_COM1, logLevelColorsEscCode[logLevel]);
			serial.writeString(SERIAL_PORT_COM1, buffer + 10);
			serial.writeString(SERIAL_PORT_COM1, "\x1b[0m");
		#else
			serial.writeString(SERIAL_PORT_COM1, buffer);
		#endif
	#endif
	#ifdef LOG_COLOR
		uint32_t origColor = TTY_Color;
		TTY_Color = colors.vga.yellow;
		putchar('[');
		TTY_Color = colors.vga.lcyan;
		for (uint8_t i = 1; i != 9; i++) {
			putchar(buffer[i]);
		}
		TTY_Color = colors.vga.yellow;
		putchar(']');
		putchar(' ');
		TTY_Color = logLevelColorsFrameBuf[logLevel];
		puts(buffer + 11);
		TTY_Color = origColor;
	#else
		puts(buffer);
	#endif
	free(buffer);
}