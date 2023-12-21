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


/*
	Ok so this is how the PMM works:
	There are CONFIG_MAX_MEMBLK number of starting "memblk"s.
	Each memblk follows the struct below this comment.

	A memblk points to the next memblk by keeping the size of the block.

	If the block is measured in pages, the data will begin at the next
	page-aligned address after the block, and the next will begin 
	at that address + (numPages * PAGE_SIZE) + 1.
	There will be a pointer to the start of this block's metadata,
	along with a magic number at the end of the padding page.


	If the block is measured in bytes, the data will begin immediately
	after the end of the block metadata, and will end at
	start of block metadata + sizeof(memblk_t) + numBytes + 1.

	To allocate a block:
	  - Start at one of the starting memblk's
	  - Follow it with the rules above.
	  - Find a valid free block with enough free bytes (or free pages)
	  - If one is found, mark it as used, set the size and pages flags accordingly, then follow the rules above for figuring out the pointer to return.
	  - If you encounter a NULL before finding a free block, try the next starting block and repeat

	To free a block:
	  - Check a few bytes back.  Do we have the pointer magic, or block magic?
	  - If we have the block magic, we have a bytes allocation, and we can just mark it as free
	  	- NOTE: Check for other free blocks before and after this to see if we can merge them!
	  - If we have the pointer magic, we need to go to the pointer value.
	  - Now we have the block, follow the same steps as for freeing a bytes block.
*/

#define BLOCK_MAGIC 0x4F6F
#define PTR_MAGIC   0x6F4F
typedef struct {
	uint16_t magicNum;
	void *   ptr;
} blockPtr_t;
typedef struct {
	uint16_t magicNum			: 16;
	uint8_t  isFree             : 1;
	uint8_t  isNumPages         : 1;
	uint64_t numBytesOrNumPages : 46;
} memblk_t;
static memblk_t *memblks[CONFIG_MAX_MEMBLK];


#if CONFIG_PMM_DEBUG == 1
#define DBG_printf printf
#define DBG_log log
#else
#define DBG_printf(x, ...) (void)0
#define DBG_log(x, y) (void)0
#endif

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

	uint_fast8_t memblkIndex = 0;
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

			memblks[memblkIndex]  = cur.start;
			*memblks[memblkIndex] = (memblk_t){.magicNum = BLOCK_MAGIC, .isFree = true, .isNumPages = true, .numBytesOrNumPages = (uint64_t)numPages};
			memblkIndex++;
			memblks[memblkIndex] = NULL;
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

	return NULL;
}

void *PMM_AllocBytes(size_t bytes) {
	DBG_log("PMM_AllocBytes", LOGLEVEL_VERBOSE);
	if (bytes == 0) {
		log("Refusing to allocate 0 bytes of memory.", LOGLEVEL_WARN);
		return NULL;
	}

	for (int i = 0; i != CONFIG_MAX_MEMBLK; i++) {
		memblk_t *current = memblks[i];

		DBG_printf("current: %p\r\n", current);
		if (current == NULL) {
			log("Reached end of memblks list and haven't found free block big enough. OOM?", LOGLEVEL_FATAL);
			return NULL;
		}
		while (*(uint64_t *)current != 0) {
			if (current->magicNum != BLOCK_MAGIC) {
				log("Block magic invalid!!  Skipping to next memblk.", LOGLEVEL_WARN);
				break;
			}
			(void)PTR_MAGIC;

			size_t nbytes = current->numBytesOrNumPages;
			DBG_printf("orignbytes: %zu\r\n", nbytes);
			uint8_t *currentTmp = (uint8_t *)current;
			if (current->isNumPages != 0) {
				nbytes *= 4096;
			}


			if (!current->isFree) {
				// not free, skip
				DBG_log("used blk", LOGLEVEL_VERBOSE);
				goto nextblk;
			}


			if (nbytes <= bytes) {
				// block too small
				DBG_log("too small", LOGLEVEL_VERBOSE);
				goto nextblk;
			}

			DBG_log("Found free block", LOGLEVEL_VERBOSE);
			current->isFree = false;
			DBG_printf("numBytesOrPages: %zu\r\n", current->numBytesOrNumPages);
			current->numBytesOrNumPages = bytes;
			current->isNumPages = false;
			DBG_printf("numBytesOrPages2: %zu\r\n", current->numBytesOrNumPages);

			void *addr = currentTmp + sizeof(memblk_t) + 1;
			if (addr == NULL) {
				DBG_log("what", LOGLEVEL_FATAL);
				while (true) {}
			}

			currentTmp += bytes + sizeof(memblk_t) + 1;
			DBG_printf("currentTmp: %p\r\nbytes: %zu\r\n", currentTmp, bytes);
			current = (memblk_t *)currentTmp;

			current->magicNum = BLOCK_MAGIC;
			current->isFree = true;
			
			// make a new block
			return addr;


			nextblk:;
			DBG_log("nextblk", LOGLEVEL_VERBOSE);
			DBG_printf("current2: %p\r\nnbytes: %zu\r\n", current, nbytes);
			currentTmp += nbytes + 1;
			current = (memblk_t *)currentTmp;
			DBG_printf("current3: %p\r\n", current);
		}
	}
	return NULL;
}

void PMM_Free(void *ptr) {
	if (ptr == NULL) {
		log("Refusing to free NULL pointer.", LOGLEVEL_WARN);
		return;
	}
}
#else
#warning "CONFIG_PMM != 1.  No memory management will be available!"
#endif