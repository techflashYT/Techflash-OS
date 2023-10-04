#include <kernel/mem.h>
#include <kernel/bootloader.h>
#include <kernel/registers.h>
#include <kernel/panic.h>
#include <stddef.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
MODULE("PMM");


// we keep a copule blocks in BSS just to have something to dump the info to,
// since we can't allocate more memory without it
static meminfoBlk_t memInfoBlocks[32];

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

		memset(memInfoBlocks, 0, sizeof(memInfoBlocks));

		if (cur.type == MM_TYPE_FREE) {
			totalUsableBytes += cur.size;
			usableIdx++;

			int index = usableIdx;
			usableIdx++;

			memInfoBlocks[index].start = cur.start;
			memInfoBlocks[index].size = cur.size;
			memInfoBlocks[index].type = MEMINF_TYPE_FREE;
			if (usableIdx >= 32) {
				log(MODNAME, "Exceeded 32 usable memory blocks during init.  Hoping we have enough to allocate, and trying to start over.", LOGLEVEL_FATAL);
				goto endLoop;
			}
			memInfoBlocks[index + 1].type = MEMINF_TYPE_END;

		}
endLoop:
		sprintf(str, "Entry %d%s: %p - %p; %-9s Type: %s", i, space, cur.start, cur.start + cur.size, sizeStr, typeStr);
		log(MODNAME, str, LOGLEVEL_DEBUG);
	}
	PMM_CalcSizeStr(sizeStr, totalUsableBytes);
	sprintf(str, "Total usable memory: %s", sizeStr);
	log(MODNAME, str, LOGLEVEL_DEBUG);
	
	log(MODNAME, "PMM Initialized!", LOGLEVEL_INFO);
}

/*
Should be handled like this
- Use a stack mechanism to make use of the `MEMINF_TYPE_INFOPTR` block (popping off the stack if it was unable to find a free block before encountering a `MEMINF_TYPE_END`)
- Using that capability if necessary, find a free block of the correct size, recursing down any info blocks if necessary
- If a block was found that is larger than the allocation requires, we should modify the block in order to mark it as used, and have the correct size, then make a new "free" block to mark any extra space afterwards as used.
- If after looking at every block, it was still unable to find a block of at least the correct size, give up, make a log message, and return NULL.
*/
static void *memInfoStack[32];
static int   memInfoStackTop = 0;

static int shiftAndCreate(meminfoBlk_t *curBlk, size_t newSize) {
	// Reduce the size of the current block
	size_t oldSize = curBlk->size;
	curBlk->size = newSize;
	curBlk->type = MEMINF_TYPE_USED;

	// Find the first empty block
	int emptyIndex = -1;
	for (int i = 1;; i++) {
		if (curBlk[i].type == MEMINF_TYPE_EMPTY) {
			emptyIndex = i;
			break;
		}
		if (curBlk[i].type == MEMINF_TYPE_END) {
			// no empty blocks, give up
			return 1;
		}
	}

	// Shift all blocks after the current block to the right until reaching the empty block
	for (int i = emptyIndex; i > 1; i--) {
		curBlk[i] = curBlk[i - 1];
	}

	// Create a new block with the remaining size
	curBlk[1].size = oldSize - newSize;
	curBlk[1].type = MEMINF_TYPE_FREE;
	return 0;
}

void *PMM_Alloc(size_t pages) {
	void *ret;
	if (pages == 0) {
		log(MODNAME, "Refusing to allocate 0 pages of memory.", LOGLEVEL_WARN);
		return NULL;
	}
	meminfoBlk_t *curInfoBlk = memInfoBlocks;
	while (true) {
		if (curInfoBlk->type == MEMINF_TYPE_USED || curInfoBlk->type == MEMINF_TYPE_EMPTY) {
			curInfoBlk = &(curInfoBlk[1]);
			continue;
		}
		else if (curInfoBlk->type == MEMINF_TYPE_FREE) {
			// found a free block!  But is it big enough?
			if (curInfoBlk->size == pages) {
				// yes, exactly big enough!  just set it to used and return it
				curInfoBlk->type = MEMINF_TYPE_USED;
				ret = curInfoBlk->start;
				goto leave;
			}
			else if (curInfoBlk->size > pages) {
				// bigger than we need.  reduce the size of curInfoBlk to what is necessary,
				// find the first empty block, shift all blocks after curInfoBlk to the right until reaching that empty block
				// make the new block, set it to free, and set the size to the difference between the original curInfoBlk's size, and the new one that we just set as used. 
				if (shiftAndCreate(curInfoBlk, pages) == 1) {
					log(MODNAME, "Failed to shift mem control blocks over - No empty blocks", LOGLEVEL_ERROR);
					continue;
				}
			}
		}
		else if (curInfoBlk->type == MEMINF_TYPE_INFOPTR) {
			// push the next block to check onto the stack
			memInfoStack[memInfoStackTop] = &(curInfoBlk[1]);
			memInfoStackTop++;
			curInfoBlk = curInfoBlk->start;
			continue;
		}
		else if (curInfoBlk->type == MEMINF_TYPE_END) {
			// end of this block, but we might be in part of a stack
			// check if we are in a stack, if we are, pop out of it
			// if we're at the lowest level and still hit an end without
			// finding any free blocks, we can give up and return NULL

			if (memInfoStackTop > 0) {
				// we are in a stack, pop out of it and continue
				curInfoBlk = memInfoStack[memInfoStackTop];
				
				// drop our stack level
				memInfoStackTop--;
				
				// continue parsing the next highest level block
				continue;
			}

			// nope, we're at the bottom of the stack, lets get out of here
			ret = NULL;
			goto leave;
		}
		else {
			// we've encountered some nonsense data
			// we've either ran past the end of the array somehow,
			// or the memory is corrupted
			// either way, this is very bad, panic
			registers_t regs;
			char tmp[64];
			
			DUMPREGS(&regs);
			
			snprintf(tmp, sizeof(tmp), "Invalid memory control block type: 0x%X", curInfoBlk->type);
			panic(tmp, &regs);
		}
		curInfoBlk = &(curInfoBlk[1]);
	}

leave:
	memInfoStackTop = 0;
	return ret;
}

void PMM_Free(void *ptr) {
	(void)ptr;
}