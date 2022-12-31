#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <kernel/memory.h>
#include <kernel/misc.h>
#include <kernel/panic.h>
#include <stdlib.h>
#if 0
void testMalloc() {
	printf("====== STARTING MALLOC TEST 1 ======\r\n");
	printf("THIS TEST SHOULD NOT CAUSE LEAKS!\r\n\r\n");
	
	void *initPtr1 = malloc(64);
	void *lastPointer = NULL;
	for (uint32_t i = 0; i < 10000; i++) {
		printf("malloc iteration %d\r", i);
		void *pointer = malloc(64);
		lastPointer = pointer;
		free(pointer);
	}
	printf("\r\n\r\n======= MALLOC TEST 1 RESULTS ======\r\n\r\n");
	printf("initial pointer: 0x%x\r\n", initPtr1);
	printf("final pointer:   0x%x\r\n\r\n\r\n", lastPointer);

	printf("====== STARTING MALLOC TEST 2 ======\r\n");
	printf("THIS TEST **SHOULD** CAUSE LEAKS!\r\n");
	printf("THIS TEST **SHOULD** CAUSE A **64 BYTE** LEAK **EVERY ITERATION**!\r\n\r\n");
	void *initPtr2 = malloc(64);
	void *lastPointer2 = NULL;
	for (uint32_t i = 0; i < 10000; i++) {
		printf("malloc iteration %d\r", i);
		void *pointer = malloc(64);
		void *pointer2 = malloc(64);
		lastPointer2 = pointer;
		free(pointer);
	}
	printf("\r\n\r\n======= MALLOC TEST 2 RESULTS ======\r\n\r\n");
	printf("initial pointer: 0x%x\r\n", initPtr2);
	printf("final pointer:   0x%x\r\n", lastPointer2);
	asm("cli\nhlt");
}
#endif

void *alignedPtr(void *ptr) {
	while ((uintptr_t) ptr % sizeof(int) != 0) {
		ptr++;
	}

	return ptr;
}
void *malloc(size_t size) {
	if (!heapSetUp) {
		lastValidHeapAddress = heapSpace;
		heapSetUp = true;
	}


	unsigned char *currentPtr = heapSpace;
	void *allocatedLocation = 0;

	size += sizeof(memControlBlock);
	memControlBlock *mcb = (memControlBlock *)currentPtr;

	if (haveAllocated) {
		while (currentPtr != lastValidHeapAddress) {
			if (mcb->free && mcb->size >= size) {
				mcb->free = false;
				allocatedLocation = currentPtr;
				break;
			}

			currentPtr += mcb->size;
			currentPtr = (unsigned char*)alignedPtr(currentPtr);
			// comout((const char*)currentPtr);
			//printf("%p\r\n", currentPtr);
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
	if (allocatedLocation < 0x0000000001000000) {
		DUMPREGS
		panic("uh oh malloc took a crap", regs);
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
	if (!ptr) {
		return;
	}

	memControlBlock *mcb = (memControlBlock *)((char*)ptr - sizeof(memControlBlock));
	if (!mcb->size) {
		return;
	}

	mcb->free = true;
}

void mallocInit(unsigned char* addr) {
	heapSpace = addr;
}