#include <kernel/panic.h>
#include <kernel/hardware/serial.h>
#include <stdio.h>
#include <stddef.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <kernel/memory.h>
// #if 0
void testMalloc() {
	uint8_t buf[64];
	/*
	printf("====== STARTING MALLOC TEST 1 ======\r\n");
	printf("THIS TEST SHOULD NOT CAUSE LEAKS!\r\n\r\n");
	
	void *initPtr1 = malloc(64);
	if (initPtr1 == NULL) {
		puts("ERROR: malloc failed after just the first test!");
		while (true) {asm("cli;hlt");}
	}
	void *lastPointer = NULL;
	for (uint32_t i = 0; i < 10000; i++) {
		printf("malloc iteration %d\r", i);
		void *pointer = malloc(64);
		if (pointer == NULL) {
			printf("malloc returned NULL on test 1, iteration %d", i);
			while (true) {asm("cli;hlt");}
		}
		memset(pointer, 0, 64);
		memcpy(buf, pointer, 64);
		if (memcmp(pointer, buf, 64) != 0) {
			puts("\e[31mERROR: content does not match!");
			while (true) {asm("cli;hlt");}
		}

		lastPointer = pointer;
		free(pointer);
	}
	puts("\r\n\r\n======= MALLOC TEST 1 RESULTS ======\r\n\r\n");
	printf("initial pointer: %p\r\n", initPtr1);
	printf("final pointer:   %p\r\n\r\n\r\n", lastPointer);
*/

	printf("====== STARTING MALLOC TEST 2 ======\r\n");
	printf("THIS TEST \e[1mSHOULD\e[0m CAUSE LEAKS!\r\n");
	printf("THIS TEST \e[1mSHOULD\e[0m CAUSE A \e[1m64 BYTE\e[0m LEAK \e[1mEVERY ITERATION\e[0m!\r\n\r\n");
	void *initPtr2 = malloc(64);
	void *lastPointer2 = NULL;
	for (uint32_t i = 0; i < 10000; i++) {
		// printf("\rmalloc iteration %d", i);
		void *pointer = malloc(64);
		memset(pointer, 0, 64);
		memcpy(buf, pointer, 64);
		if (memcmp(pointer, buf, 64) != 0) {
			puts("\e[31mERROR:` content does not match!");
			asm("cli;hlt");
		}

		void *pointer2 = malloc(64);
		if (pointer2 <= pointer + 63) {
			puts("\e[31mERROR: malloc gave us the a pointer less than 64 bytes after the last one\r\n");
			asm("cli;hlt");
		}
		memset(pointer2, 0, 64);
		memcpy(buf, pointer2, 64);
		if (memcmp(pointer2, buf, 64) != 0) {
			puts("\e[31mERROR: content does not match!");
			asm("cli;hlt");
		}
		lastPointer2 = pointer;
		free(pointer);
		(void)pointer2;
	}
	printf("\r\n\r\n======= MALLOC TEST 2 RESULTS ======\r\n\r\n");
	printf("initial pointer: %p\r\n", initPtr2);
	printf("final pointer:   %p\r\n", lastPointer2);
	// asm("cli\nhlt");
}
// #endif

void *malloc(size_t size) {
	return PMM_Alloc(ALIGN_PAGE(size));
}
void free(void *p) {
	PMM_Free(p);
}