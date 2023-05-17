#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <kernel/hardware/serial.h>
#include <kernel/graphics.h>
#include <kernel/tty.h>
#define LOG_TO_SERIAL
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
void log(const char *module, const char* message, uint_fast8_t logLevel) {
	if (logLevel < kernel.logLevel) {
		return;
	}
	// colorize using actual ANSI escape sequences for the serial logging
	char *buffer = "\x1b[0m\x1b[33m[\x1b[1m\x1b[36m        \x1b[0m\x1b[33m]\x1b[0m ";
	memcpy(buffer + 19, module, strlen(module));


	#ifdef LOG_TO_SERIAL
		serial.writeString(SERIAL_PORT_COM1, buffer);
		serial.writeString(SERIAL_PORT_COM1, logLevelColorsEscCode[logLevel]);
		serial.writeString(SERIAL_PORT_COM1, message);
		serial.writeString(SERIAL_PORT_COM1, "\x1b[0m");
	#endif
	uint32_t origColor = TTY_Color;
	puts(buffer);
	TTY_Color = logLevelColorsFrameBuf[logLevel];
	puts(message);
	puts("\r\n");
	TTY_Color = origColor;
	free(buffer);
}