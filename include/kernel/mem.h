#include <stdio.h>
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

// reclaimable memory
#define MM_FLAG_RECLAIM  0x1
// bad memory, do not touch
#define MM_FLAG_BAD_MEM  0x2
// read only memory, don't write here
#define MM_FLAG_READONLY 0x4
// memory with framebuffer
#define MM_FLAG_FRAMEBUF 0x8
// the memory where the kernel binary lives
#define MM_FLAG_KERNEL   0x10
// the memory where modules live
#define MM_FLAG_MODULES  0x20

typedef struct {
	uint8_t numEntries;
	struct {
		uint8_t type;
		uint8_t flags;
		void   *start;
		size_t  size;
	} entries[];	
} memmap_t;


extern void PMM_Init();
