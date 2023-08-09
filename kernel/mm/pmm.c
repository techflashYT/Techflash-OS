#include <kernel/mem.h>
#include <kernel/bootloader.h>
#include <kernel/registers.h>
#include <kernel/panic.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
MODULE("PMM");

static uint8_t *bitmap;



static bitmapData_t bitmapData[CONFIG_KERN_MAX_BITMAPDATA];
static uint8_t numBitmapData = 0;


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

	uint64_t bitmapPages = ALIGN_PAGE(totalUsableBytes);
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

	sprintf(str, "Found region %d, initializing bitmap on it.", usableRegions[bitmapRegion]);
	log(MODNAME, str, LOGLEVEL_DEBUG);

	bitmap = memmap->entries[usableRegions[bitmapRegion]].start;
	sprintf(str, "bitmap address: %p", bitmap);
	log(MODNAME, str, LOGLEVEL_DEBUG);

	for (uint8_t i = 0; i != numBitmapData; i++) {
		// TODO: set bitmapData info
	}
	(void)bitmapData;
}