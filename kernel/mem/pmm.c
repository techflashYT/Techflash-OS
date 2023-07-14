#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <kernel/panic.h>
#include <kernel/environment.h>
#include <kernel/hardware/serial.h>
#include <kernel/memory.h>

// #pragma GCC optimize("O1")

#define PAGE_SIZE ((uint64_t)4096)
#define PHYS_MEM_SIZE ((uint64_t)1 << 30)  // 1 GB of physical memory
#define NUM_PAGES (PHYS_MEM_SIZE / PAGE_SIZE)

// Declare a bitmap for tracking the state of each page
static uint64_t *PMM_FreePagesBitmap = NULL;

// Define a struct to represent a page header
typedef struct PageHeader {
	uint8_t order;
	bool allocated;
	struct PageHeader *next;
} PageHeader;

// Declare an array of pointers to page headers for each block size
static PageHeader **PMM_Headers = NULL;

// Declare a free page list for each block size
static PageHeader *PMM_FreePagesList[11] = { NULL };

typedef struct {
	void *addr;
	uint64_t size;
} usableMemEntry;
usableMemEntry entries[16];

// Initialize the physical memory manager
void PMM_Init(void) {
	entries[15].addr = (void *)0xDEADBEEF;
	entries[15].size = 0;
	uint_fast8_t bootbootEntries = (bootboot.size - 128) / 16;
	MMapEnt *mmap = &bootboot.mmap;
	uint_fast64_t totalUsableMem = 0;
	uint_fast8_t memEntryIndex = 0;
	puts("====== MEMORY MAP ======\r\n");

	for (uint_fast8_t i = 0; i != bootbootEntries; i++) {
		void *ptr = (void *)MMapEnt_Ptr(&mmap[i]);
		uint_fast64_t size = MMapEnt_Size(&mmap[i]);
		void *endptr = size + ptr;
		uint_fast8_t type = MMapEnt_Type(&mmap[i]);

		char *free = "no";
		if (MMapEnt_IsFree(&mmap[i])) {
			free = "yes";
			entries[memEntryIndex].addr = ptr;
			entries[memEntryIndex].size = size;
			totalUsableMem += size;
			memEntryIndex++;
			entries[memEntryIndex].addr = (void *)0xDEADBEEF;
			entries[memEntryIndex].size = 0;
		}
		char *typeStr;
		if (type == MMAP_USED) {typeStr = "Used";}
		if (type == MMAP_FREE) {typeStr = "Free";}
		if (type == MMAP_ACPI) {typeStr = "ACPI";}
		if (type == MMAP_MMIO) {typeStr = "MMIO";}

		char sizeStr[12];
		double sizeFlt = size;
		for (uint_fast8_t i = 0; i != 5; i++) {
			if ((sizeFlt / 1024) > 100) {
				sizeFlt /= 1024;
				continue;
			}
			// not greater than 100*unit*, let's convert it
			char unit = '?';
			if (i == 0) {unit = 'B';}
			if (i == 1) {unit = 'K';}
			if (i == 2) {unit = 'M';}
			if (i == 3) {unit = 'G';}
			if (i == 4) {unit = 'T';}
			sprintf(sizeStr, "%.2f%c", sizeFlt, unit);
			break;
		}

		printf("Entry %d: %p - %p; Size: %s; Type: %s (%d); Free?: %s\r\n", i, ptr, endptr, sizeStr, typeStr, type, free);
	}
	printf("%ldKB usable memory.\r\n", totalUsableMem / 1024);
	puts("====== USABLE ======\r\n");
	
	int biggest = 15;
	for (uint_fast8_t i = 0; entries[i].addr != (void *)0xDEADBEEF; i++) {
		char sizeStr[12];
        double sizeFlt = entries[i].size;
        for (uint_fast8_t i = 0; i != 5; i++) {
            if ((sizeFlt / 1024) > 100) {
                sizeFlt /= 1024;
                continue;
            }
            // not greater than 100*unit*, let's convert it
            char unit = '?';
            if (i == 0) {unit = 'B';}
            if (i == 1) {unit = 'K';}
            if (i == 2) {unit = 'M';}
            if (i == 3) {unit = 'G';}
            if (i == 4) {unit = 'T';}
            sprintf(sizeStr, "%.2f%c", sizeFlt, unit);
            break;
        }
		printf("Entry %d: %p - %p; Size: %s\r\n", i, entries[i].addr, entries[i].addr + entries[i].size, sizeStr);
		if (entries[i].size > entries[biggest].size) {
			biggest = i;
		}
	}
	printf("biggest entry: %d\r\nInitializing allocator on it\r\n", biggest);
	mallocInit(entries[biggest].addr);
	/*
	// Dynamically allocate the array of pointers to page headers
	PMM_Headers = malloc(11 * sizeof(PageHeader *));
	if (PMM_Headers == NULL) {
		DUMPREGS;
		panic("PMM: Failed to allocate PMM_Headers", regs);
	}

	// Dynamically allocate the actual page headers for each block size
	for (uint8_t i = 0; i < 11; i++) {
		uint64_t page_size = ((uint64_t)1 << i);
		uint64_t num_headers = NUM_PAGES / page_size;
		PMM_Headers[i] = malloc(num_headers * sizeof(PageHeader));
		if (PMM_Headers[i] == NULL) {
			DUMPREGS;
			panic("PMM: Failed to allocate PMM_Headers", regs);
		}
		memset(PMM_Headers[i], 0, num_headers * sizeof(PageHeader));
		for (uint64_t j = 0; j < num_headers; j++) {
			PMM_Headers[i][j].order = i;
			PMM_FreePagesList[i] = &PMM_Headers[i][j];
			if (j < num_headers - 1) {
				PMM_Headers[i][j].next = &PMM_Headers[i][j + 1];
			}
		}
	}


	// Dynamically allocate the free page bitmap and initialize it to zero
	PMM_FreePagesBitmap = malloc(NUM_PAGES / (sizeof(uint64_t) * 8) * sizeof(uint64_t));
	if (PMM_FreePagesBitmap == NULL) {
		DUMPREGS;
		panic("PMM: Failed to allocate PMM_FreePagesBitmap", regs);
	}
	for (uint64_t i = 0; i < NUM_PAGES / (sizeof(uint64_t) * 8); i++) {
		PMM_FreePagesBitmap[i] = 0;
	}*/
}

// Allocate a page of physical memory
void *PMM_AllocatePage(void) {
	uint8_t i;

	// Search for an available block of the smallest size with a free page
	for (i = 0; i < 11; i++) {
		if (PMM_FreePagesList[i] != NULL) {
			PageHeader *header = PMM_FreePagesList[i];
			PMM_FreePagesList[i] = header->next;
			header->allocated = true;

			uint64_t physicalAddress = (uint64_t)header + ((uint64_t)1 << i) - PAGE_SIZE;
			uint64_t pageIndex = physicalAddress / PAGE_SIZE;
			PMM_FreePagesBitmap[pageIndex / (sizeof(uint64_t) * 8)] |= ((uint64_t)1 << (pageIndex % (sizeof(uint64_t) * 8)));

			return (void *)physicalAddress;
		}
	}

	return NULL;
}

// Free a page of physical memory
void PMM_FreePage(void *page) {
	uint64_t pageIndex = (uint64_t)page / PAGE_SIZE;
	PMM_FreePagesBitmap[pageIndex / (sizeof(uint64_t) * 8)] &= ~((uint64_t)1 << (pageIndex % (sizeof(uint64_t) * 8)));
	PageHeader *header = (PageHeader *)page;
	header->allocated = false;
	header->next = PMM_FreePagesList[header->order];
	PMM_FreePagesList[header->order] = header;
}

// Free the dynamically allocated memory for the free page bitmap and the page headers
void PMM_Destroy(void) {
	for (uint8_t i = 0; i < 11; i++) {
		free(PMM_Headers[i]);
	}
	free(PMM_Headers);
	PMM_Headers = NULL;

	free(PMM_FreePagesBitmap);
	PMM_FreePagesBitmap = NULL;
}