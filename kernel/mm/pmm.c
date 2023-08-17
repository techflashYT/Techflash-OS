#include <kernel/mem.h>
#include <kernel/bootloader.h>
#include <kernel/registers.h>
#include <kernel/panic.h>
#include <stddef.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
MODULE("PMM");


// NOTE: In the bitmap, 0 means free, 1 means used.
static uint8_t *bitmap;
static size_t   bitmapPages;


static bitmapData_t bitmapData[CONFIG_KERN_MAX_BITMAPDATA];
static uint8_t numBitmapData = 0;


static bool PMM_CheckBitmapSet(size_t byteOffset, uint_fast8_t bitOffset) {
	return (bitmap[byteOffset] & (1 << bitOffset));
}

static void *PMM_BitmapToAddress(size_t byteOffset, uint_fast8_t bitOffset) {
	// use bitmapData struct to find the address of the bit in the bitmap
	/* STEPS:
		1. Find the first entry where our bit is higher than the .endingBit value
		2. Use the previous entry, since it's somewhere in there
		3. Figure out the starting bit of that entry with the following logic:
			- if the entry number is 0, the starting bit is 0
			- if the entry number is greater than 0, the starting bit is the .endingBit value of the previous entry + 1.
		4. Figure out the starting bit + the offset in bits, converted to an offset in address (multiple by 8 * 4096)
		5. Return that address
	*/
	int correctEntry = 0;

	if ((byteOffset * 8) + bitOffset > bitmapData[0].endingBit) {
		for (correctEntry = 0; correctEntry < numBitmapData; correctEntry++) {
			if (bitmapData[correctEntry].endingBit > bitOffset) {
				break;
			}
		}
		correctEntry--;
	}
	

	size_t addr   = (size_t)bitmapData[correctEntry].basePtr;
	size_t offset = (((byteOffset * 8) + bitOffset) * 4096);
	return (void *)(addr + offset);
}

ssize_t PMM_AddressToBitmap(void *addr) {
	for (size_t i = 0; i < numBitmapData; i++) {
		if (bitmapData[i].basePtr >= addr) {
			i--;
			return (ssize_t)(bitmapData[i].endingBit - ALIGN_PAGE((size_t)addr));
		}
	}
	return -1000000;
}
extern memmap_t *LM_ParseMemmap();
static memmap_t *BOOT_ParseMemmap() {
	if (BOOT_LoaderID == BOOT_LoaderID_LimineCompatible) {
		return LM_ParseMemmap();
	}
	registers_t regs;
	DUMPREGS(&regs);
	log(MODNAME, "Unkown Bootloader!  No idea where the memory map is.  Giving up.", LOGLEVEL_FATAL);
	panic("No memory map detected", &regs);
	return NULL;
}

static void PMM_CalcSizeStr(char *sizeStr, size_t size) {
	uint_fast8_t i;
	size_t oldSize = 0;
	size_t decimal = 0;
	char *types = "BKMGTP???";
	for (i = 0; i != 6; i++) {
		if ((size % 1024) == 0) {
			oldSize = size;
			size /= 1024;
			continue;
		}
		if ((size / 1024) >= 10) {
			oldSize = size;
			size /= 1024;
			continue;
		}
		break;
	}
	decimal = oldSize % 1024;
	if (decimal > 0) {
		sprintf(sizeStr, "%ld.%ld%c", size, (decimal * 100) / 1024, types[i]);
	}
	else {
		sprintf(sizeStr, "%ld%c", size, types[i]);
	}
}
static char *typeStrs[] = {
	"Usable", "Reserved", "ACPI",
	"Memory Mapped I/O", "Bootloader",
};

static char *flagsStrs[] = {
	"Reclaimable", "Bad Memory", "Read Only",
	"Framebuffer", "Kernel", "Modules",
};
void PMM_Init() {
	log(MODNAME, "PMM Initializing", LOGLEVEL_INFO);
	log(MODNAME, "Getting memory map from bootloader...", LOGLEVEL_DEBUG);
	char str[128];
	char sizeStr[16];
	uint64_t totalUsableBytes = 0;
	uint_fast8_t usableIdx = 0;
	int usableRegions[16] = {0xFFFF};

	memmap_t *memmap = BOOT_ParseMemmap();

	sprintf(str, "Got memory map with %lu entries at %p", memmap->numEntries, memmap);
	log(MODNAME, str, LOGLEVEL_DEBUG);
	
	log(MODNAME, "====== Memory Map ======", LOGLEVEL_DEBUG);

	for (uint_fast8_t i = 0; i != memmap->numEntries; i++) {
		char typeStr[128];
		memmapEntry_t cur = memmap->entries[i];

		// Make size & type strings
		PMM_CalcSizeStr(sizeStr, cur.size);
		strcat(sizeStr, ";");

		strcpy(typeStr, typeStrs[cur.type]);
		for (uint_fast8_t j = 0; j != 6; j++) {
			if (cur.flags & (1 << j)) {sprintf(typeStr + strlen(typeStr), " (%s)", flagsStrs[j]);}
		}
		
		char *space = "";
		if ((memmap->numEntries >= 10) && (i < 10)) {space = " ";}

		if (cur.type == MM_TYPE_FREE) {
			totalUsableBytes += cur.size;
			usableRegions[usableIdx] = i;
			usableIdx++;
		}

		sprintf(str, "Entry %d%s: %p - %p; %-9s Type: %s", i, space, cur.start, cur.start + cur.size, sizeStr, typeStr);
		log(MODNAME, str, LOGLEVEL_DEBUG);
	}
	PMM_CalcSizeStr(sizeStr, totalUsableBytes);
	sprintf(str, "Total usable memory: %s", sizeStr);
	log(MODNAME, str, LOGLEVEL_DEBUG);
	
	strcpy(str, "Usable regions: ");
	bool first = true;
	for (uint_fast8_t i = 0; i != usableIdx; i++) {
		if (!first) {strcat(str, ", ");}
		sprintf(str + strlen(str), "%d", usableRegions[i]);
		first = false;
	}
	log(MODNAME, str, LOGLEVEL_DEBUG);

	bitmapPages = ALIGN_PAGE(totalUsableBytes);
	sprintf(str, "Making bitmap of size %lu pages (%lu bytes)", bitmapPages, bitmapPages / 8);
	log(MODNAME, str, LOGLEVEL_DEBUG);

	log(MODNAME, "Finding the smallest region big enough to hold the bitmap...", LOGLEVEL_DEBUG);
	int bitmapRegion = -1;
	
	for (uint_fast8_t i = 0; i < usableIdx; i++) {
		int regionIndex = usableRegions[i];
		if (memmap->entries[regionIndex].size >= (bitmapPages / 8)) {
			if (bitmapRegion == -1 || memmap->entries[regionIndex].size < memmap->entries[usableRegions[bitmapRegion]].size) {
				bitmapRegion = i;
			}
		}
		numBitmapData++;
	}
	
	if (bitmapRegion == -1) {
		registers_t regs;
		DUMPREGS(&regs);
		log(MODNAME, "Couldn't find a region big enough to hold the bitmap!", LOGLEVEL_FATAL);
		panic("All memory regions too small to hold bitmap", &regs);
	}

	bitmapRegion = usableRegions[bitmapRegion];
	sprintf(str, "Found region %d, initializing bitmap on it.", bitmapRegion);
	log(MODNAME, str, LOGLEVEL_DEBUG);

	bitmap = memmap->entries[bitmapRegion].start;
	sprintf(str, "bitmap address: %p", bitmap);
	log(MODNAME, str, LOGLEVEL_DEBUG);

	for (uint8_t i = 0; i != numBitmapData; i++) {
		size_t size = memmap->entries[usableRegions[i]].size;

		bitmapData[i].basePtr = memmap->entries[usableRegions[i]].start;
		bitmapData[i].endingBit = size / PAGE_SIZE;

		// if (usableRegions[i] == bitmapRegion) {
			// bitmapData[i].basePtr = (void *)(ALIGN_PAGE(((size_t)bitmap + bitmapPages / 8)) * 4096);
			// size -= bitmapPages / 8;
		// }

		for (int j = i - 1; j > 0; j--) {
			bitmapData[i].endingBit += bitmapData[j].endingBit;
		}
		sprintf(str, "bitmapData[%d] = {.basePtr = %p, .endingBit = %lu}", i, bitmapData[i].basePtr, bitmapData[i].endingBit);
		log(MODNAME, str, LOGLEVEL_VERBOSE);
	}

	log(MODNAME, "Bitmap Data saved.  Setting all pages to free...", LOGLEVEL_DEBUG);
	memset(bitmap, 0x00, bitmapPages / 8);
	log(MODNAME, "Done.  Setting bitmap pages to used...", LOGLEVEL_DEBUG);
	memset(bitmap, 0xFF, ALIGN_PAGE(bitmapPages / 8));

	log(MODNAME, "PMM Initialized!", LOGLEVEL_INFO);
}

void *PMM_Alloc(size_t pages) {
	if (pages == 0) {
		log(MODNAME, "Refusing to allocate 0 pages of memory.", LOGLEVEL_WARN);
		return NULL;
	}
	/*
		TODO: the requirement for contiguous memory here is bad.
		Find a way to allow piecing together different chunks of memory, probably using paging.
	*/
	// find a section of memory that is at least `pages` long (each bit of the bitmap represents 1 page)
	
	// find the first bit in the bitmap that isn't set
	char str[32];
	sprintf(str, "finding %zu pages", pages);
	log(MODNAME, str, LOGLEVEL_DEBUG);
	size_t freeSize = 0;
	for (size_t i = 0; i != bitmapPages / 8; i++) {
		for (uint_fast8_t j = 0; j != 8; j++) {
			if (!PMM_CheckBitmapSet(i, j)) {
				// hey we found a free page!
				// log(MODNAME, "Found a free page!", LOGLEVEL_DEBUG);
				freeSize++;
				// have we found enough free pages yet?
				if (freeSize == pages) {
					// YES!  Return the memory we found.
					void *ptr = PMM_BitmapToAddress(i, j);
					// set the pages to used
					for (size_t k = 0; k != pages; k++) {
						bitmap[i] |= (1 << j);
					}
					return ptr;
				}
				continue;
			}
			// this page was used.  Set freeSize to 0 so we can keep going until we find the next block.
			// log(MODNAME, "Page was used.", LOGLEVEL_DEBUG);
			freeSize = 0;
		}
	}
	// we ran out of pages in the bitmap without finding a chunk of free memory big enough.
	return NULL;

	// TODO: if we get here, we should find non-contiguous memory and piece it together with paging, but that's for later.
}

void PMM_Free(void *ptr) {
	printf("PMM_Free: Address to bitmap position: %zd\r\n", PMM_AddressToBitmap(ptr));
}