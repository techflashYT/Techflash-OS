#include <kernel/COM.h>
#include <kernel/SMP.h>
#include <kernel/tty.h>
#include <kernel/mem.h>
#include <kernel/arch.h>
#include <kernel/bootloader.h>
#include <kernel/tty/fbCon.h>
#include <stdio.h>
#include <string.h>

void testMalloc();
void __attribute__((noreturn)) main() {
	TTY_SetWriteFunc(COM_LogWrapper);
	BOOT_CheckLoader();
	SMP_Init();
	ARCH_Init();
	PMM_Init();

	FBCON_Init();


	/*
	puts("Testing PMM!\r\n");

	void *ptr = PMM_Alloc(5);
	void *ptr2 = PMM_Alloc(5);
	printf("Recieved pointers: %p and %p\r\n", ptr, ptr2);
	memset(ptr, 0xA5, 5 * 4096);
	memset(ptr2, 0xA5, 5 * 4096);

	PMM_Free(ptr);
	PMM_Free(ptr2);

	ptr2 = PMM_Alloc(5);
	printf("Recieved pointers: %p and %p\r\n", ptr, ptr2);

	*/
	testMalloc();
	

	puts("Kernel execution done.  Hanging.\r\n");
	while (true) {asm ("cli; hlt");}
}