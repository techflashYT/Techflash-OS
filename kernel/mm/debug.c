#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MALLOC_DEBUG
#ifdef MALLOC_DEBUG
#define NUM_TESTS 1000
#define PAGE_SIZE 4096
#define PROG_BAR_WIDTH 20


// Techflash OS has a weird puts that doesn't put a newline
// this app was built around it, so we emulate it here
#ifndef __TFOS__
#include <stdlib.h>
#include <stdbool.h>
static void custom_puts(char *str) {
	while (*str != '\0') {
		putchar(*str);
		str++;
	}
}
#define puts custom_puts
#define FAIL exit(1);
#endif


// building for Techflash OS, define FAIL as halt
#ifdef __TFOS__
#define FAIL while (true) { asm("cli; hlt");}
#endif


static int previousProgress = 0;
static bool last = true;
static void handleProgress(int prog, int max) {
	int currentProgress = prog / (max / PROG_BAR_WIDTH);
	if (currentProgress != previousProgress) {
		if (!last) {
			for (int i = 0; i != PROG_BAR_WIDTH + 3; i++) {
				putchar('\b');
			}
		}
		else {
			last = false;
		}
		putchar('[');
		for(int i = 0; i < PROG_BAR_WIDTH; i++) {
			putchar((currentProgress >= i) ? '#' : ' ');
		}
		puts("] ");
		previousProgress = currentProgress;
	}
}
static void printRed(const char* message) {
	printf("\e[1m\e[31m%s\e[0m\r\n", message);
}
static void printGreen(const char* message) {
	printf("\e[1m\e[32m%s\e[0m\r\n", message);
}
static void __attribute__((unused)) printYellow(const char* message) {
	printf("\e[1m\e[33m%s\e[0m\r\n", message);
}

#define PASS printGreen("PASS"); last = true;

char* alloc[NUM_TESTS];
#ifdef __TFOS__
void checkContiguous(int test_num, void* block1, void* block2) {
	size_t diff = (size_t)block2 - (size_t)block1;
	if (diff != PAGE_SIZE) {
		printRed("Error: Contiguous memory check failed!");
		printf("Test Number: %d\r\n", test_num);
		printf("Block 1 address: %p\r\n", block1);
		printf("Block 2 address: %p\r\n", block2);
		printf("Expected difference: %d\r\n", PAGE_SIZE);
		printf("Actual difference: %zu\r\n", diff);
		FAIL
	}

	if ((size_t)block1 % PAGE_SIZE != 0 || (size_t)block2 % PAGE_SIZE != 0) {
		printRed("Error: Contiguous memory check failed due to page alignment!");
		printf("Test Number: %d\r\n", test_num);
		printf("Block 1 address: %p\r\n", block1);
		printf("Block 2 address: %p\r\n", block2);
		printf("Page size: %d\r\n", PAGE_SIZE);
		FAIL
	}
}
#endif
void testMalloc() {
	for (int goodNum = 0; goodNum != NUM_TESTS;) {
		puts("Test #1: Basic memory allocation and deallocation: ");
		for (int i = 0; i < NUM_TESTS; i++) {
			// Test 1: Basic memory allocation and deallocation
			handleProgress(i, NUM_TESTS);
			int* ptr1 = (int*)malloc(sizeof(int));
			if (ptr1 == NULL) {
				printRed("FAIL\r\nError: malloc() returned NULL on test #1!");
				FAIL
			}
			*ptr1 = 42;
			free(ptr1);
		}
		PASS

		puts("Test #2: Allocating and freeing large chunks of memory: ");
		for (int i = 0; i < NUM_TESTS / 6; i++) {
			// Test 2: Allocating and freeing large chunks of memory
			handleProgress(i, NUM_TESTS / 6);
			size_t tenMB = 1024 * 1024 * 10;
			char* ptr2 = (char*)malloc(tenMB);
			if (ptr2 == NULL) {
				printRed("FAIL\r\nError: malloc() returned NULL on test #2!");
				FAIL
			}
			memset(ptr2, 0, tenMB);
			free(ptr2);
		}
		PASS

		puts("Test #3: Allocate memory, modify, and check: ");
		for (int i = 0; i < NUM_TESTS; i++) {
			// Test 3: Allocate memory, modify, and check
			handleProgress(i, NUM_TESTS);
			int* ptr3 = (int*)malloc(sizeof(int));
			if (ptr3 == NULL) {
				printRed("FAIL\r\nError: malloc() returned NULL on test #3!");
				FAIL
			}
			*ptr3 = 100;
			if (*ptr3 != 100) {
				printRed("FAIL\r\nError: Memory modification check failed!");
				FAIL
			}
			free(ptr3);
		}
		PASS

		puts("Test #4: Allocate array of chars and manipulate the content: ");

		for (int i = 0; i < NUM_TESTS; i++) {
			// Test 4: Allocate array of chars and manipulate the content
			handleProgress(i, NUM_TESTS);
			char* str = (char*)malloc(20);
			if (str == NULL) {
				printRed("Error: malloc() returned NULL on test #4!");
				FAIL
			}
			strcpy(str, "Hello, ");
			strcat(str, "World!");
			if (strcmp(str, "Hello, World!") != 0) {
				printRed("Error: String manipulation check failed!");
				FAIL
			}
			free(str);
		}
		PASS

		puts("Test #5: Allocate multiple blocks and check for contiguous memory: ");
#ifndef __TFOS__
		printYellow("Test skipped - Not in kernel");
		last = true;
#else
		for (int i = 0; i < NUM_TESTS; i++) {
			// Test 5: Allocate multiple blocks and check for contiguous memory
			handleProgress(i, NUM_TESTS);
			int* block1 = (int*)malloc(sizeof(int));
			int* block2 = (int*)malloc(sizeof(int));
			checkContiguous(i, block1, block2);
			free(block1);
			free(block2);
		}
		PASS
#endif

		size_t prev_end = 0;

		puts("Test #6: Check that next alloc is a higher address than prev + Test #7: Ensure no two allocated ranges overlap: ");
#ifndef __TFOS__
		printYellow("Test #6 skipped - Not in kernel");
#endif
		for (int j = 0; j < NUM_TESTS; j++) {
			alloc[j] = (char*)malloc(1024);
			if (alloc[j] == NULL) {
				printRed("Error: malloc() returned NULL on test #5!");
				FAIL
			}

#ifdef __TFOS__

			if (j > 0) {
				if (((size_t)alloc[j] - prev_end) < PAGE_SIZE) {
					printRed("Error: Next allocation is not further than the previous one!");
					printf("Current allocation address: %p\r\n", alloc[j]);
					printf("Previous allocation address: %p\r\n", (void*)prev_end);
					FAIL
				}
			}
#endif


			for (int k = 0; k < j; k++) {
				if ((size_t)alloc[k] < (size_t)alloc[j] + 1024 &&
					(size_t)alloc[j] < (size_t)alloc[k] + 1024) {
					printRed("Error: Allocated ranges overlap!");
					printf("Overlap detected between block %d and block %d\r\n", k, j);
					printf("Block %d address: %p\r\n", k, alloc[k]);
					printf("Block %d address: %p\r\n", j, alloc[j]);
					FAIL
				}
			}
			prev_end = (size_t)alloc[j];
		}

		// Free allocated memory
		for (int j = 0; j < NUM_TESTS; j++) {
			free(alloc[j]);
		}

		goodNum++;
		// All tests passed for this run
		printf("Tests completed: %d/%d successful runs\r\n", goodNum, NUM_TESTS);
	}

}
// not under Techflash OS, we probably want a main function
// since we're probably building this as a standalone program
#ifndef __TFOS__
int main() {
	testMalloc();
}
#endif
#endif
