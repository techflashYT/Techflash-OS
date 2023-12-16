#if CONFIG_PMM == 1

#include <kernel/mem.h>
#include <kernel/bootloader.h>
#include <kernel/registers.h>
#include <kernel/panic.h>
#include <stddef.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
MODULE("PMM");

typedef struct {
	uint8_t  isFree             : 1;
	uint8_t  isNumPages         : 1;
	uint64_t numBytesOrNumPages : 62;
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
	"BIOS Data Area", "Framebuffer", "Kernel", "Modules",
};
void PMM_Init() {
	log("PMM Initializing", LOGLEVEL_INFO);
	log("Getting memory map from bootloader...", LOGLEVEL_DEBUG);
	char str[128];
	char sizeStr[16];
	uint64_t totalUsableBytes = 0;

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

			memblks[memblkIndex]  = cur.start;
			*memblks[memblkIndex] = (memblk_t){.isFree = true, .isNumPages = true, .numBytesOrNumPages = (uint16_t)numPages};
			memblkIndex++;
		}
		
		sprintf(str, "Entry %d%s: %p - %p; %-9s Type: %s", i, space, cur.start, cur.start + cur.size, sizeStr, typeStr);
		log(str, LOGLEVEL_DEBUG);
	}
	PMM_CalcSizeStr(sizeStr, totalUsableBytes);
	sprintf(str, "Total usable memory: %s", sizeStr);
	log(str, LOGLEVEL_DEBUG);
	
	log("PMM Initialized!", LOGLEVEL_INFO);
	return;
}

void *PMM_Alloc(size_t pages) {
    if (pages == 0) {
        log("Refusing to allocate 0 pages of memory.", LOGLEVEL_WARN);
        return NULL;
    }

    for (int i = 0; i < 64; i++) {
        memblk_t *blk = memblks[i];
        while (blk != NULL && blk->isFree && blk->isNumPages && blk->numBytesOrNumPages >= pages) {
            if (blk->numBytesOrNumPages > pages) {
                // Split the block
                memblk_t *newBlk = (memblk_t *)((char *)blk + (pages + 1) * PAGE_SIZE);
                newBlk->isFree = true;
                newBlk->isNumPages = true;
                newBlk->numBytesOrNumPages = blk->numBytesOrNumPages - pages - 1;

                // Update the original block
                blk->isFree = false;
                blk->numBytesOrNumPages = pages;
            } else {
                // The block is exactly the right size
                blk->isFree = false;
            }

            // Return a pointer to the allocated block
            return (void *)((char *)blk + PAGE_SIZE);
        }

        // Move to the next block
        blk = (memblk_t *)((char *)blk + (blk->numBytesOrNumPages + 1) * (blk->isNumPages ? PAGE_SIZE : 1));
    }

    log("Failed to allocate memory: not enough free space.", LOGLEVEL_ERROR);
    return NULL;
}

void *PMM_AllocBytes(size_t bytes) {
    if (bytes == 0) {
        log("Refusing to allocate 0 bytes of memory.", LOGLEVEL_WARN);
        return NULL;
    }

    for (int i = 0; i < 64; i++) {
        memblk_t *blk = memblks[i];
        while (blk != NULL && blk->isFree && !blk->isNumPages && blk->numBytesOrNumPages >= bytes) {
            if (blk->numBytesOrNumPages > bytes) {
                // Split the block
                memblk_t *newBlk = (memblk_t *)((char *)blk + bytes + 1);
                newBlk->isFree = true;
                newBlk->isNumPages = false;
                newBlk->numBytesOrNumPages = blk->numBytesOrNumPages - bytes - 1;

                // Update the original block
                blk->isFree = false;
                blk->numBytesOrNumPages = bytes;
            } else {
                // The block is exactly the right size
                blk->isFree = false;
            }

            // Return a pointer to the allocated block
            return (void *)((char *)blk + 1);
        }

        // Move to the next block
        blk = (memblk_t *)((char *)blk + (blk->numBytesOrNumPages + 1) * (blk->isNumPages ? PAGE_SIZE : 1));
    }

    log("Failed to allocate memory: not enough free space.", LOGLEVEL_ERROR);
    return NULL;
}

void PMM_Free(void *ptr) {
    if (ptr == NULL) {
        log("Refusing to free NULL pointer.", LOGLEVEL_WARN);
        return;
    }

    for (int i = 0; i < 64; i++) {
        memblk_t *blk = memblks[i];
        while (blk != NULL) {
            if ((void *)((char *)blk + (blk->isNumPages ? PAGE_SIZE : 1)) == ptr) {
                // Found the block to be freed
                blk->isFree = true;

                // Merge with previous block if it's free
                if (i > 0 && memblks[i - 1]->isFree) {
                    memblks[i - 1]->numBytesOrNumPages += blk->numBytesOrNumPages + 1;
                    blk = memblks[i - 1];
                }

                // Merge with next block if it's free
                memblk_t *nextBlk = (memblk_t *)((char *)blk + (blk->numBytesOrNumPages + 1) * (blk->isNumPages ? PAGE_SIZE : 1));
                if (nextBlk != NULL && nextBlk->isFree) {
                    blk->numBytesOrNumPages += nextBlk->numBytesOrNumPages + 1;
                }

                return;
            }

            // Move to the next block
            blk = (memblk_t *)((char *)blk + (blk->numBytesOrNumPages + 1) * (blk->isNumPages ? PAGE_SIZE : 1));
        }
    }
}
#else
#warning "CONFIG_PMM != 1.  No memory management will be available!"
#endif