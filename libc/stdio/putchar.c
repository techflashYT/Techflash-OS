#include <stdlib.h>
#include <string.h>
#include <kernel/font.h>
#include <kernel/environment.h>
#include <kernel/tty.h>
#include <kernel/panic.h>
#include <kernel/hardware/IO.h>
#include <kernel/hardware/PIT.h>
#include <kernel/hardware/serial.h>
extern bool timerReady;
int putchar(const int ch) {
	flanterm_write(TTY_Ctx, (char []){ch, '\0'}, 1);
	return ch;
}
