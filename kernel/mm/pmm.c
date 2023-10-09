#include <kernel/mem.h>
#include <kernel/bootloader.h>
#include <kernel/registers.h>
#include <kernel/panic.h>
#include <stddef.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
MODULE("PMM");

static bitmap_t bitmaps[32];

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


		if (cur.type == MM_TYPE_FREE) {
			totalUsableBytes += cur.size;


			// Calculate the number of pages in this block
			size_t numPages = cur.size / PAGE_SIZE;

			// Allocate space for the bitmap
			bitmap_t *bitmap = &(bitmaps[usableIdx]);

			bitmap->bits = cur.start;
			bitmap->size = (numPages + 7) / 8;

			// Calculate the number of pages used by the bitmap
			size_t bitmapPages = ALIGN_PAGE(bitmap->size);

			// Mark the pages used by the bitmap as used
			for (size_t j = 0; j < bitmapPages; j++) {
				bitmap->bits[j / 64] |= (1ULL << (j % 64));
			}


			if (usableIdx >= 32) {
				log("Exceeded 32 usable memory blocks during init.  Hoping we have enough to allocate, and trying to start over.", LOGLEVEL_FATAL);
				goto endLoop;
			}

			usableIdx++;
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

	for (int idx = 0; idx < 32; idx++) {
		bitmap_t *bitmap = &(bitmaps[idx]);
		for (size_t i = 0; i < bitmap->size; i++) {
			if (!(bitmap->bits[i / 64] & (1ULL << (i % 64)))) {
				// Found a free page, mark it as used
				bitmap->bits[i / 64] |= (1ULL << (i % 64));

				// Return the address of the page
				return (void *)((uintptr_t)bitmap->bits + i * PAGE_SIZE);
			}
		}
	}
	return NULL;
}

void PMM_Free(void *ptr) {
	for (int idx = 0; idx < 32; idx++) {
		bitmap_t *bitmap = &(bitmaps[idx]);
		// Calculate the page index
		size_t i = ((uintptr_t)ptr - (uintptr_t)bitmap->bits) / PAGE_SIZE;

		// Check if the pointer falls within this bitmap
		if (((uintptr_t)ptr > (uintptr_t)bitmap->bits) && ((uintptr_t)ptr < ((uintptr_t)bitmap->bits + (bitmap->size * PAGE_SIZE)))) {
			// Mark the page as free
			bitmap->bits[i / 64] &= ~(1ULL << (i % 64));
			return;
		}
	}
	log("Attempted to free a pointer not managed by the PMM.", LOGLEVEL_ERROR);
}