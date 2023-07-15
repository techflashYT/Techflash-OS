#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <kernel/memory.h>
#include <kernel/panic.h>
#include <kernel/hardware/serial.h>
#include <stdlib.h>
#pragma GCC optimize("O0")
// #if 0
#include <stdio.h>
void testMalloc() {
	uint8_t buf[64];
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
	printf("\r\n\r\n======= MALLOC TEST 1 RESULTS ======\r\n\r\n");
	printf("initial pointer: %p\r\n", initPtr1);
	printf("final pointer:   %p\r\n\r\n\r\n", lastPointer);

	printf("====== STARTING MALLOC TEST 2 ======\r\n");
	printf("THIS TEST \e[1mSHOULD\e[0m CAUSE LEAKS!\r\n");
	printf("THIS TEST \e[1mSHOULD\e[0m CAUSE A \e[1m64 BYTE\e[0m LEAK \e[1mEVERY ITERATION\e[0m!\r\n\r\n");
	void *initPtr2 = malloc(64);
	void *lastPointer2 = NULL;
	for (uint32_t i = 0; i < 10000; i++) {
		printf("malloc iteration %d\r", i);
		void *pointer = malloc(64);
		memset(pointer, 0, 64);
		memcpy(buf, pointer, 64);
		if (memcmp(pointer, buf, 64) != 0) {
			puts("\e[31mERROR: content does not match!");
			asm("cli;hlt");
		}

		void *pointer2 = malloc(64);
		if (pointer2 <= pointer) {
			puts("\e[31mERROR: malloc gave us the same or less value pointer than before\r\n");
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


void *alignedPtr(void *ptr) {
	while ((uintptr_t) ptr % sizeof(int) != 0) {
		ptr++;
	}

	return ptr;
}
void *malloc(size_t size) {
	{
		char str[16];
		sprintf(str, "malloc(%ld)\r\n", size);
		serial.writeString(SERIAL_PORT_COM1, str);
	}
	unsigned char *currentPtr = heapSpace;
	if (heapSpace < (uint8_t *)0x10) {
		DUMPREGS;
		panic("malloc(): heapSpace is BS", regs);
	}
	void *allocatedLocation = heapSpace;

	size += sizeof(memControlBlock);
	memControlBlock *mcb = (memControlBlock *)currentPtr;

	if (haveAllocated) {
		while (currentPtr > lastValidHeapAddress) {
			if (mcb->free && mcb->size >= size) {
				mcb->free = false;
				allocatedLocation = currentPtr;
				break;
			}

			currentPtr += mcb->size;
			currentPtr = (unsigned char*)alignedPtr(currentPtr);
		}
	}

	if (!allocatedLocation) {
		allocatedLocation = lastValidHeapAddress;
		lastValidHeapAddress += size;

		mcb = (memControlBlock*)allocatedLocation;
		mcb->free = false;
		mcb->size = size;
	}

	allocatedLocation += sizeof(memControlBlock);
	
	if (allocatedLocation < (void *)0x1000) {
		DUMPREGS
		panic("uh oh malloc took a crap", regs);
	}
	{
		char str[32];
		sprintf(str, "ret: %p\r\n", allocatedLocation);
		serial.writeString(SERIAL_PORT_COM1, str);
	}
	return allocatedLocation;
}

void *realloc(void *ptr, size_t size) {
	if (!ptr) {
		return malloc(size);
	}

	if (!size && ptr) {
		free(ptr);
		return NULL;
	}

	void *newPtr = malloc(size);

	memControlBlock *mcb = (memControlBlock *)(ptr - sizeof(memControlBlock));

	memcpy(newPtr, ptr, mcb->size);
	free(ptr);

	return newPtr;
}

void free(void* ptr) {
	char str[32];
	sprintf(str, "free(%p)\r\n", ptr);
	serial.writeString(SERIAL_PORT_COM1, str);
	if (!ptr) {
		return;
	}

	volatile memControlBlock *mcb = (memControlBlock *)((char*)ptr - sizeof(memControlBlock));
	if ((size_t)(&mcb + 8)) {
		return;
	}

	mcb->free = true;
	serial.writeString(SERIAL_PORT_COM1, "block freed\r\n");
}

void mallocInit(uint8_t *addr, uint_fast64_t size) {
	if (addr < (uint8_t *)0x1000) {
		DUMPREGS;
		panic("mallocInit(): Called with address below 0x1000", regs);
	}
	char str[48];
	sprintf(str, "mallocInit(): addr=%p\r\n", addr);
	serial.writeString(SERIAL_PORT_COM1, str);
	heapSpace = addr;
	lastValidHeapAddress = heapSpace + size;
	heapSetUp = true;
}
