#include <kernel/COM.h>
#include <kernel/SMP.h>
#include <kernel/tty.h>
#include <kernel/mem.h>
#include <kernel/arch.h>
#include <kernel/bootloader.h>
#include <stdio.h>


void main() {
	TTY_SetWriteFunc(COM_LogWrapper);
	BOOT_CheckLoader();
	SMP_Init();
	ARCH_Init();
	PMM_Init();

	puts("Kernel execution done.  Hanging.\r\n");
	while (true) {asm ("cli; hlt");}
}