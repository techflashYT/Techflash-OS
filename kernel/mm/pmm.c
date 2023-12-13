#include <kernel/mem.h>
#include <kernel/bootloader.h>
#include <kernel/registers.h>
#include <kernel/panic.h>
#include <stddef.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
MODULE("PMM");

typedef struct {
	uint8_t  isFree             : 1;
	uint8_t  isNumPages         : 1;
	uint16_t numBytesOrNumPages : 14;
} memblk_t;
static memblk_t *memblks[64];

extern memmap_t *LM_ParseMemmap();
static memmap_t *BOOT_ParseMemmap() {
	if (BOOT_LoaderID == BOOT_LoaderID_LimineCompatible) {
		return LM_ParseMemmap();
	}
	registers_t regs;
	DUMPREGS(&regs);
	log("Unkown Bootloader!  No idea where the memory map is.  Giving up.", LOGLEVEL_FATAL);
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
	log("PMM Initializing", LOGLEVEL_INFO);
	log("Getting memory map from bootloader...", LOGLEVEL_DEBUG);
	char str[128];
	char sizeStr[16];
	uint64_t totalUsableBytes = 0;
	uint_fast8_t usableIdx = 0;

	memmap_t *memmap = BOOT_ParseMemmap();

	sprintf(str, "Got memory map with %lu entries at %p", memmap->numEntries, memmap);
	log(str, LOGLEVEL_DEBUG);
	
	log("====== Memory Map ======", LOGLEVEL_DEBUG);

	for (uint_fast8_t i = 0; i != memmap->numEntries; i++) {
		char typeStr[128];
		memmapEntry_t cur = memmap->entries[i];

		// Make size & type strings
		PMM_CalcSizeStr(sizeStr, cur.size);
		strcat(sizeStr, ";");

		strcpy(typeStr, typeStrs[cur.type]);
		for (uint_fast8_t j = 0; j != 6; j++) {
			if (cur.flags & (1 << j)) {
				sprintf(typeStr + strlen(typeStr), " (%s)", flagsStrs[j]);
			}
		}
		
		char *space = "";
		if ((memmap->numEntries >= 10) && (i < 10)) {space = " ";}


		uint_fast8_t memblkIndex = 0;
		if (cur.type == MM_TYPE_FREE) {
			totalUsableBytes += cur.size;

			// Calculate the number of pages in this block
			size_t numPages = cur.size / PAGE_SIZE;

			memblks[memblkIndex] = 
			memblkIndex++;
		}
endLoop:
		sprintf(str, "Entry %d%s: %p - %p; %-9s Type: %s", i, space, cur.start, cur.start + cur.size, sizeStr, typeStr);
		log(str, LOGLEVEL_DEBUG);
	}
	PMM_CalcSizeStr(sizeStr, totalUsableBytes);
	sprintf(str, "Total usable memory: %s", sizeStr);
	log(str, LOGLEVEL_DEBUG);
	
	log("PMM Initialized!", LOGLEVEL_INFO);
}

void *PMM_Alloc(size_t pages) {
	if (pages == 0) {
		log("Refusing to allocate 0 pages of memory.", LOGLEVEL_WARN);
		return NULL;
	}

	return NULL;
}

void PMM_Free(void *ptr) {
	log(MODNAME, "Attempted to free a pointer not managed by the PMM.", LOGLEVEL_ERROR);
}