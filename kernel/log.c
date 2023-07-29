#include <stdio.h>
#include <kernel/COM.h>
#include <kernel/tty.h>
void log(const char module[8], const char *msg, const uint8_t level) {
	(void)level;
	// NOTE: we do this so that GCC can check the format
	#define fmt "\e[1;33m[\e[36m%s\e[1;33m]\e[0; %s"

	// are we still in early boot where the write func is set to serial?
	if (((void *)TTY_WriteFunc) != ((void *)COM_LogWrapper)) {
		// no, write it to serial manually, as well as to whatever the current output is
		char str[256];
		sprintf(str, fmt, module, msg);
		COM_WriteStr(0, str);
		puts(str);
		return;
	}
	// yes, just write it to serial
	printf(fmt, module, msg);
}