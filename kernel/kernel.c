#include <kernel/COM.h>
#include <kernel/SMP.h>
#include <kernel/tty.h>
#include <kernel/arch.h>

void main() {
	TTY_SetWriteFunc(COM_LogWrapper);
	SMP_Init();
	ARCH_Init();
	*(uint8_t *)0x000 = 0x10;
	while (true) {}
}