#include <kernel/COM.h>
#include <kernel/SMP.h>
#include <kernel/tty.h>
#include <kernel/mem.h>
#include <kernel/arch.h>
#include <kernel/bootloader.h>
#include <kernel/tty/fbCon.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>


void testMalloc();
void __attribute__((noreturn)) main() {
	TTY_SetWriteFunc(COM_LogWrapper);
	printf("Techflash OS Version %s loading...\r\n", VER_STR);
	BOOT_CheckLoader();
	SMP_Init();
	ARCH_Init();

	#if CONFIG_PMM == 1
	PMM_Init();
	#endif

	#if CONFIG_FBCON == 1
	FBCON_Init();
	#endif


	puts("Testing PMM!\r\n");

	void *ptr = PMM_AllocBytes(5);
	void *ptr2 = PMM_AllocBytes(5);
	printf("Recieved pointers: %p and %p\r\n", ptr, ptr2);
	memset(ptr, 0xA5, 5);
	memset(ptr2, 0xA5, 5);

	PMM_Free(ptr);
	PMM_Free(ptr2);

	ptr2 = PMM_Alloc(5);
	printf("Recieved pointers: %p and %p\r\n", ptr, ptr2);

	// testMalloc();
	

	puts("Kernel execution done.  Hanging.\r\n");
	while (true) {asm ("cli; hlt");}
}