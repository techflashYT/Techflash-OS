#include <stdio.h>
#include <stdint.h>
// free RAM, use this however
#define MM_TYPE_FREE 0
// reserved, we can't do anything with this (e.g. used by BIOS)
#define MM_TYPE_RSRV 1
// ACPI stuff
#define MM_TYPE_ACPI 2
// Memory Mapped I/O
#define MM_TYPE_MMIO 3
// Bootloader stuff, might be freeable, make sure to check flags
#define MM_TYPE_BOOT 4
// unknown, we have no idea what this is
#define MM_TYPE_UNKN 0xFF

// reclaimable memory
#define MM_FLAG_RECLAIM  0x1
// bad memory, do not touch
#define MM_FLAG_BAD_MEM  0x2
// read only memory, don't write here
#define MM_FLAG_READONLY 0x4
// BIOS Data Area, only used on BIOS/CSM machines
#define MM_FLAG_BDA      0x8
// memory with framebuffer
#define MM_FLAG_FRAMEBUF 0x10
// the memory where the kernel binary lives
#define MM_FLAG_KERNEL   0x20
// the memory where modules live
#define MM_FLAG_MODULES  0x40


// these are the types they are to allow for proper alignment
typedef struct {
	uint32_t type;
	uint32_t flags;
	void    *start;
	size_t   size;
} memmapEntry_t;

typedef struct {
	uint64_t numEntries;
	memmapEntry_t entries[];
} memmap_t;


extern void  PMM_Init ();
extern void *PMM_Alloc(size_t pages);
extern void *PMM_AllocBytes(size_t bytes);
extern void  PMM_Free (void *ptr);


#define PAGE_SIZE 4096
#define NUM_PAGES(x) ((x + 4095) / 4096)

typedef struct {
	uint64_t *bits;
	size_t size;
} bitmap_t;
