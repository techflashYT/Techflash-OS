#include <stdio.h>
#include <string.h>
#include <kernel/COM.h>
#include <kernel/tty.h>
static char *ansiColors[] = {
	"\e[0m",       // verbose
	"\e[0m",       // debug
	"\e[37m",      // info
	"\e[33m",      // warn
	"\e[0;31m",    // error
	"\e[31m !!! "  // fatal
};
void log(const char module[8], const char *msg, const uint8_t level) {
	(void)level;
	// NOTE: we do this so that GCC can check the format
	#define fmt "\e[1;33m[\e[36m%-*s\e[1;33m]%s %s", 8

	// are we still in early boot where the write func is set to serial?
	char str[256];
	sprintf(str, fmt, module, ansiColors[level], msg);
	if (level == LOGLEVEL_FATAL) {
		strcat(str, " !!!");
	}
	strcat(str, "\e[0m\r\n");

	if (((void *)TTY_WriteFunc) != ((void *)COM_LogWrapper)) {
		// no, write it to serial manually, as well as to whatever the current output is
		COM_WriteStr(0, str);
	}
	// yes, just write it to serial
	puts(str);
}