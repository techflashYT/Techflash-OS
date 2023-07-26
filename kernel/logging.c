#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <kernel/hardware/serial.h>
#include <kernel/graphics.h>
#include <kernel/tty.h>
#define LOG_TO_SERIAL
static char *logLevelColorsEscCode[] = {
	"\e[0m",         // verbose
	"\e[0m",         // debug
	"\e[0m\e[1;39m", // info
	"\e[0m\e[33m",   // warn
	"\e[0m\e[31m",   // err
	"\e[0m\e[31m"    // fatal
};
static uint32_t logLevelColorsFrameBuf[] = {
	0x00AAAAAA,   // verbose
	0x00AAAAAA,   // debug
	0x00DDDDDD,   // info
	warningColor, // warn
	errorColor,   // err
	errorColor    // fatal
};
colorChar_t *logBuffer;
void log(const char *module, const char* message, uint_fast8_t logLevel) {
	if (logLevel < kernel.logLevel) {
		return;
	}
	char buffer[256];
	sprintf(buffer, "\e[0m\e[33m[\e[1m\e[36m%-8s\e[0m\e[33m]\e[0m ", module);


	#ifdef LOG_TO_SERIAL
		serial.writeString(SERIAL_PORT_COM1, buffer);
		serial.writeString(SERIAL_PORT_COM1, logLevelColorsEscCode[logLevel]);
		serial.writeString(SERIAL_PORT_COM1, message);
		serial.writeString(SERIAL_PORT_COM1, "\e[0m\r\n");
	#endif
	uint32_t origColor = TTY_Color;
	puts(buffer);
	TTY_Color = logLevelColorsFrameBuf[logLevel];
	if (logLevel == LOGLEVEL_FATAL) {
		puts("!!!! ");
	}

	puts(message);
	if (logLevel == LOGLEVEL_FATAL) {
		puts(" !!!!");
	}
	puts("\e[0m\r\n");
	TTY_Color = origColor;
	FB_Update();
}