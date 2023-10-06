#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <kernel/COM.h>
#define MALLOC_DEBUG
#ifdef MALLOC_DEBUG
#define NUM_TESTS 1000
#define PAGE_SIZE 4096

static uint_fast8_t dotsPos = 0;
static void handleDots() {
	if (dotsPos == 3) {
		puts("\b\b\b   \b\b\b");
		dotsPos = 0;
		return;
	}
	putchar('.');
	dotsPos++;
}
static void resetDots() {
	for (uint_fast8_t i = 0; i != dotsPos; i++) {
		puts("\b \b");
	}
	dotsPos = 0;
}
static void printRed(const char* message) {
	resetDots();
	printf("\e[1m\e[31m%s\e[0m\r\n", message);
}
static void printGreen(const char* message) {
	resetDots();
	printf("\e[1m\e[32m%s\e[0m\r\n", message);
}


char* alloc[NUM_TESTS];
void checkContiguous(int test_num, void* block1, void* block2) {
	size_t diff = (size_t)block2 - (size_t)block1;
	if (diff != PAGE_SIZE) {
		printRed("Error: Contiguous memory check failed!");
		printf("Test Number: %d\r\n", test_num);
		printf("Block 1 address: %p\r\n", block1);
		printf("Block 2 address: %p\r\n", block2);
		printf("Expected difference: %d\r\n", PAGE_SIZE);
		printf("Actual difference: %zu\r\n", diff);
		while (true) { asm("cli;hlt"); }
	}

	if ((size_t)block1 % PAGE_SIZE != 0 || (size_t)block2 % PAGE_SIZE != 0) {
		printRed("Error: Contiguous memory check failed due to page alignment!");
		printf("Test Number: %d\r\n", test_num);
		printf("Block 1 address: %p\r\n", block1);
		printf("Block 2 address: %p\r\n", block2);
		printf("Page size: %d\r\n", PAGE_SIZE);
		while (true) { asm("cli;hlt"); }
	}
}
void testMalloc() {
	int goodNum = 0;

	puts("Test #1: Basic memory allocation and deallocation: ");
	for (int i = 0; i < NUM_TESTS; i++) {
		// Test 1: Basic memory allocation and deallocation
		handleDots();
		int* ptr1 = (int*)malloc(sizeof(int));
		if (ptr1 == NULL) {
			printRed("FAIL\r\nError: malloc() returned NULL on test #1!");
			while (true) { asm("cli;hlt"); }
		}
		*ptr1 = 42;
		free(ptr1);
	}
	printGreen("PASS");

	puts("Test #2: Allocating and freeing large chunks of memory: ");
	for (int i = 0; i < NUM_TESTS / 2; i++) {
		// Test 2: Allocating and freeing large chunks of memory
		handleDots();
		size_t tenMB = 1024 * 1024 * 10;
		char* ptr2 = (char*)malloc(tenMB);
		if (ptr2 == NULL) {
			printRed("FAIL\r\nError: malloc() returned NULL on test #2!");
			while (true) { asm("cli;hlt"); }
		}
		memset(ptr2, 0, tenMB);
		free(ptr2);
	}
	printGreen("PASS");

	puts("Test #3: Allocate memory, modify, and check: ");
	for (int i = 0; i < NUM_TESTS; i++) {
		// Test 3: Allocate memory, modify, and check
		handleDots();
		int* ptr3 = (int*)malloc(sizeof(int));
		if (ptr3 == NULL) {
			printRed("FAIL\r\nError: malloc() returned NULL on test #3!");
			while (true) { asm("cli;hlt"); }
		}
		*ptr3 = 100;
		if (*ptr3 != 100) {
			printRed("FAIL\r\nError: Memory modification check failed!");
			while (true) { asm("cli;hlt"); }
		}
		free(ptr3);
	}
	printGreen("PASS");

	puts("Test #4: Allocate array of chars and manipulate the content: ");
	for (int i = 0; i < NUM_TESTS; i++) {
		// Test 4: Allocate array of chars and manipulate the content
		handleDots();
		char* str = (char*)malloc(20);
		if (str == NULL) {
			printRed("Error: malloc() returned NULL on test #4!");
			while (true) { asm("cli;hlt"); }
		}
		strcpy(str, "Hello, ");
		strcat(str, "World!");
		if (strcmp(str, "Hello, World!") != 0) {
			printRed("Error: String manipulation check failed!");
			while (true) { asm("cli;hlt"); }
		}
		free(str);
	}
	printGreen("PASS");

	puts("Test #5: Allocate multiple blocks and check for contiguous memory: ");
	for (int i = 0; i < NUM_TESTS; i++) {
		// Test 5: Allocate multiple blocks and check for contiguous memory
		handleDots();
		int* block1 = (int*)malloc(sizeof(int));
		int* block2 = (int*)malloc(sizeof(int));
		checkContiguous(i, block1, block2);
		free(block1);
		free(block2);
	}

	// Additional Checks
	size_t prev_end = 0;

	for (int j = 0; j < NUM_TESTS; j++) {
		alloc[j] = (char*)malloc(1024);
		if (alloc[j] == NULL) {
			printRed("Error: malloc() returned NULL on test #5!");
			while (true) { asm("cli;hlt"); }
		}

		// Check 1: Ensure the next allocation is further
		if (j > 0) {
			if (((size_t)alloc[j] - prev_end) < PAGE_SIZE) {
				printRed("Error: Next allocation is not further than the previous one!");
				printf("Current allocation address: %p\r\n", alloc[j]);
				printf("Previous allocation address: %p\r\n", (void*)prev_end);
				while (true) { asm("cli;hlt"); }
			}
		}

		// Check 2: Ensure no two allocated ranges overlap
		for (int k = 0; k < j; k++) {
			if ((size_t)alloc[k] < (size_t)alloc[j] + 1024 &&
				(size_t)alloc[j] < (size_t)alloc[k] + 1024) {
				printRed("Error: Allocated ranges overlap!");
				printf("Overlap detected between block %d and block %d\r\n", k, j);
				printf("Block %d address: %p\r\n", k, alloc[k]);
				printf("Block %d address: %p\r\n", j, alloc[j]);
				while (true) { asm("cli;hlt"); }
			}
		}

		prev_end = (size_t)alloc[j];
	}

	// Free allocated memory
	for (int j = 0; j < NUM_TESTS; j++) {
		free(alloc[j]);
	}

	// All tests passed for this run
	goodNum++;

	printf("Tests completed: %d/%d successful runs\r\n", goodNum, NUM_TESTS);
}
#endif