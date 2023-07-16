#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <kernel/panic.h>
#include <kernel/environment.h>
#include <kernel/hardware/serial.h>
#include <kernel/memory.h>
MODULE("MEMORY");

// #pragma GCC optimize("O1")

static size_t	memSz;
static size_t	usedBlks;
static size_t	maxBlks;
static size_t*	memoryMap;
static size_t	memoryMapSize;
static size_t*	memoryMapEnd;
static uint32_t globI;
static uint32_t globJ;
typedef struct {
	void *addr;
	size_t size;
} usableMemEntry;
typedef struct {
	void *startAddr;
	void *endAddr;
	uint64_t bitmapEnd;
	uint64_t numBlks;
} memRegion_t;

memRegion_t regions[8];
uint_fast8_t regIndex = 0;

#define BLOCK_SIZE 4096
#define BLOCKS_PER_BYTE 8
#define BLOCK_ALIGN BLOCK_SIZE
static usableMemEntry entries[16] = {{(void *)0xDEADBEEF, 0}};
static void handleUnit(char *sizeStr, size_t size) {
	double sizeFlt = size;
	for (uint_fast8_t i = 0; i != 5; i++) {
		if ((sizeFlt / 1024) > 100 || (size % 1024) == 0) {
			size /= 1024;
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
		int off = strlen(sizeStr);
		if (sizeStr[off - 2] == '0' && sizeStr[off - 3] == '0') {
			sizeStr[off - 4] = sizeStr[off - 1];
			sizeStr[off - 3] ='\0';
		}
		break;
	}
}
static void bitmapSet(int bit) {
	memoryMap[bit / 32] |= (1 << (bit % 32));
}

static void bitmapUnset(int bit) {
	memoryMap[bit / 32] &= ~ (1 << (bit % 32));
}

static bool bitmapTest(int bit) {
	return memoryMap[bit / 32] &  (1 << (bit % 32));
}
static void PMM_InitRegion(void *base, size_t size);
// static void PMM_DeInitRegion(void *base, size_t size);
static void *PMM_FindFree_s(size_t size);
// Initialize the physical memory manager
void PMM_Init(void) {
	log(MODNAME, "Init PMM", LOGLEVEL_INFO);
	entries[15].addr = (void *)0xDEADBEEF;
	entries[15].size = 0;
	uint_fast8_t numUsable = 0;
	uint_fast64_t totalUsableMem = 0;
	{
		uint_fast8_t bootbootEntries = (bootboot.size - 128) / 16;
		MMapEnt *mmap = &bootboot.mmap;
		uint_fast8_t memEntryIndex = 0;
		log(MODNAME, "====== MEMORY MAP ======", LOGLEVEL_INFO);

		for (uint_fast8_t i = 0; i != bootbootEntries; i++) {
			void *ptr = (void *)MMapEnt_Ptr(&mmap[i]);
			uint_fast64_t size = MMapEnt_Size(&mmap[i]);
			uint_fast8_t type = MMapEnt_Type(&mmap[i]);

			char *free = "no";
			if (MMapEnt_IsFree(&mmap[i])) {
				free = "yes";
				entries[memEntryIndex].addr = ptr;
				entries[memEntryIndex].size = size;
				totalUsableMem += size;
				memEntryIndex++;
				numUsable++;
			}
			char *typeStr;
			if (type == MMAP_USED) {typeStr = "Used";}
			if (type == MMAP_FREE) {typeStr = "Free";}
			if (type == MMAP_ACPI) {typeStr = "ACPI";}
			if (type == MMAP_MMIO) {typeStr = "MMIO";}

			char sizeStr[12];
			handleUnit(sizeStr, size);

			char str[128];
			sprintf(str, "Entry %d: %p - %p; Size: %s; Type: %s (%d); Free?: %s", i, ptr, size + ptr, sizeStr, typeStr, type, free);
			log(MODNAME, str, LOGLEVEL_INFO);
		}
		char str[128];
		sprintf(str, "%ldKB usable memory.", totalUsableMem / 1024);
		log(MODNAME, str, LOGLEVEL_INFO);
	}
	log(MODNAME, "====== USABLE ======", LOGLEVEL_INFO);
	
	uint_fast8_t biggest = 15;
	for (uint_fast8_t i = 0; i != numUsable; i++) {
		char sizeStr[12];
		handleUnit(sizeStr, entries[i].size);
		
		char str[128];
		sprintf(str, "Entry %d: %p - %p; Size: %s", i, entries[i].addr, entries[i].addr + entries[i].size, sizeStr);
		log(MODNAME, str, LOGLEVEL_INFO);
		if (entries[i].size > entries[biggest].size) {
			biggest = i;
		}
	}
	{
		char str[128];
		sprintf(str, "biggest entry: %d; addr: %p; Initializing allocator on it\r\n", biggest, entries[biggest].addr);
		log(MODNAME, str, LOGLEVEL_VERBOSE);
	}
	/*mallocInit(entries[biggest].addr, entries[biggest].size);
	void *a = malloc(65535);
	printf("ptr: %p\r\n", a);
	free(a);*/

	memSz = totalUsableMem;
	memoryMap = entries[biggest].addr;
	maxBlks = (memSz * 1024) / BLOCK_SIZE;
	usedBlks = maxBlks;
	memoryMapSize = maxBlks / BLOCKS_PER_BYTE;
	memoryMapEnd = (memoryMap + memoryMapSize);
 
	char str[128];
	sprintf(str, "Initializing memory bitmap at address %p, size %ld", memoryMap, memoryMapSize);
	log(MODNAME, str, LOGLEVEL_VERBOSE);
	// By default, all of memory is in use
	memset(memoryMap, 0xf, memoryMapSize);

	// init allocator on all usable regions
	// NOTE: This is why why we keep `memoryMapEnd`, so we can init
	// the allocator on that region as well, without overwriting our bitmap
	for (uint_fast8_t i = 0; i != numUsable; i++) {
		void *addr = entries[i].addr;
		size_t size = entries[i].size;
		if (entries[i].addr == memoryMap) {
			log(MODNAME, "Initializing memory in region with memmap, dirty hack by +ptr & -size", LOGLEVEL_VERBOSE);
			addr = memoryMapEnd;
			size -= memoryMapSize;
		}
		sprintf(str, "Initializing usable region at address %p, size %ld", addr, size);
		log(MODNAME, str, LOGLEVEL_VERBOSE);
		PMM_InitRegion(addr, size);
	}

	/*puts("halting to prevent panic");
	asm("cli;hlt");*/
}
static void *baseAddr(uint32_t off, uint_fast8_t bits) {
	uint64_t bitmapOff = (off * 32) + bits;
	for (uint_fast8_t i = 0; i != 7; i++) {
		if (bitmapOff > regions[i].bitmapEnd) {
			// it's the previous entry
			volatile memRegion_t reg = regions[i - 1];
			volatile void *addr = reg.startAddr;
			char str[32];
			sprintf(str, "base: %p", addr);
			log(MODNAME, str, LOGLEVEL_DEBUG);
			return (void *)addr;
		}
	}
	return NULL;
}

static void *PMM_FindFree() {
	// find the first free bit
	for (uint32_t i = 0; i < maxBlks / 32; i++) {
		if (memoryMap[i] != 0xffffffff) {
			for (int j = 0; j < 32; j++) {		// test each bit in the dword
				int bit = 1 << j;
				if (!(memoryMap[i] & bit)) {
					return i * 4 * 8 + j + baseAddr(i, j);
				}
			}
		}
	}
 
	return NULL;
}

static void PMM_InitRegion(void *base, size_t size) {
	int align = (uint64_t)base / BLOCK_SIZE;
	size_t blocks = size / BLOCK_SIZE;
	if (blocks == 0) {
		DUMPREGS;
		panic("PMM: what how do we have 0 blocks", regs);
	}
	
	char str[26];
	sprintf(str, "initreg with index %d", regIndex);
	log(MODNAME, str, LOGLEVEL_DEBUG);
	regions[regIndex].startAddr = base;
	regions[regIndex].endAddr = base + size;
	regions[regIndex].numBlks = blocks;

	if (regIndex != 0) {
		log(MODNAME, "regIndex != 0, adding previous region's blocks", LOGLEVEL_DEBUG);
		for (int_fast8_t i = regIndex - 1; i != -1; i--) {
			log(MODNAME, "loop iteration", LOGLEVEL_VERBOSE);
			blocks += regions[i].numBlks;
			sprintf(str, "adding %ld blocks from region %d", regions[i].numBlks, i);
			log(MODNAME, str, LOGLEVEL_VERBOSE);
		}
		sprintf(str, "added %ld blocks", blocks - regions[regIndex].numBlks);
		log(MODNAME, str, LOGLEVEL_DEBUG);
	}

	regions[regIndex].bitmapEnd = blocks;
	blocks = regions[regIndex].numBlks;
	for (; blocks > 0; blocks--) {
		bitmapUnset(align++);
		usedBlks--;
	}
	regIndex++;
}

/*static void PMM_DeInitRegion(void *base, size_t size) {
	DUMPREGS;
	panic("PMM_DeInitRegion: this function is broken", regs);
	int align = (uint64_t)base / BLOCK_SIZE;
	int blocks = size / BLOCK_SIZE;
 
	for (; blocks > 0; blocks--) {
		bitmapSet(align++);
		usedBlks++;
	}
}*/

void PMM_Free(void* p) {
	int frame = (uint64_t)p / BLOCK_SIZE;
 
	bitmapUnset(frame);
	usedBlks--;
}

void *PMM_Alloc(size_t size) {
	char str[32];
	sprintf(str, "alloc(%ld)\r\n", size);
	log(MODNAME, str, LOGLEVEL_VERBOSE);

	if ((maxBlks - usedBlks) <= size) {
		return 0;	// not enough space
	}

	void *frame = PMM_FindFree_s(size);
	sprintf(str, "frame: %p", frame);
	log(MODNAME, str, LOGLEVEL_VERBOSE);


	if (frame == NULL) {
		serial.writeString(SERIAL_PORT_COM1, "NOT ENOGUH SPACE\r\n");
		return 0;	// not enough space
	}

	for (uint32_t i = 0; i < size; i++) {
		bitmapSet((uint_fast64_t)frame + i);
	}

	void *addr = (void *)((uint_fast64_t)frame + (uint_fast64_t)baseAddr(globI, globJ));
	sprintf(str, "addr: %p", addr);
	log(MODNAME, str, LOGLEVEL_VERBOSE);
	usedBlks += size;

	return (void *)addr;
}

static void *PMM_FindFree_s(size_t size) {
	if (size == 0) {
		return NULL;
	}

	if (size == 1) {
		return PMM_FindFree();
	}

	for (uint32_t i = 0; i < maxBlks / 32; i++) {
		if (memoryMap[i] != 0xffffffff) {
			for (int j = 0; j < 32; j++) {	// test each bit in the dword
				int bit = 1 << j;
				if (!(memoryMap[i] & bit)) {
					int startingBit = i * 32;
					startingBit += bit;		//get the free bit in the dword at index i

					uint32_t free = 0; //loop through each bit to see if its enough space
					for (uint32_t count = 0; count <= size; count++) {

						if (!bitmapTest(startingBit + count)) {
							free++;	// this bit is clear (free frame)
						}

						if (free == size) {
							globI = i;
							globJ = j;
							return (void *)(uint64_t)(i * 4 * 8 + j); //free count==size needed; return index
						}
					}
				}
			}
		}
	}

	return NULL;
}
