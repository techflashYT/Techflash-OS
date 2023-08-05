#include <external/limine.h>
#include <kernel/mem.h>
#include <kernel/bootloader.h>
#include <kernel/registers.h>
#include <kernel/panic.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
MODULE("PMM");
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
	"Usable",
	"Reserved",
	"ACPI",
	"Memory Mapped I/O",
	"Bootloader",
};

static char *flagsStrs[] = {
	"Reclaimable",
	"Bad Memory",
	"Read Only",
	"Framebuffer",
	"Kernel",
	"Modules",
};
void PMM_Init() {
	log(MODNAME, "PMM Initializing", LOGLEVEL_INFO);
	log(MODNAME, "Getting memory map from bootloader...", LOGLEVEL_DEBUG);
	char str[128];
	uint64_t usable = 0;
	char sizeStr[16];
	uint_fast8_t usableIdx = 0;
	int usableRegions[16] = {0xFFFF};

	memmap_t *memmap = BOOT_ParseMemmap();

	sprintf(str, "Got memory map with %lu entries at %p", memmap->numEntries, memmap);
	log(MODNAME, "====== Memory Map ======", LOGLEVEL_DEBUG);
	log(MODNAME, str, LOGLEVEL_DEBUG);

	for (uint_fast8_t i = 0; i != memmap->numEntries; i++) {
		uint64_t size = memmap->entries[i].size;
		char typeStr[128];
		strcpy(typeStr, typeStrs[memmap->entries[i].type]);

		// Make size & type strings
		PMM_CalcSizeStr(sizeStr, size);
		strcat(sizeStr, ";");


		for (uint_fast8_t j = 0; j != 6; j++) {
			if (memmap->entries[i].flags & (1 << j)) {
				strcat(typeStr, " (");
				strcat(typeStr, flagsStrs[j]);
				strcat(typeStr, ")");
			}
		}

		char *space = "";
		if ((memmap->numEntries >= 10) && (i < 10)) {
			space = " ";
		}

		if (memmap->entries[i].type == MM_TYPE_FREE) {
			usable += memmap->entries[i].size;
			usableRegions[usableIdx] = i;
			usableIdx++;
		}

		sprintf(str, "Entry %d%s: %p - %p; %-9s Type: %s", i, space, memmap->entries[i].start, memmap->entries[i].start + size, sizeStr, typeStr);
		log(MODNAME, str, LOGLEVEL_DEBUG);
	}
	PMM_CalcSizeStr(sizeStr, usable);
	sprintf(str, "Total usable memory: %s", sizeStr);
	log(MODNAME, str, LOGLEVEL_DEBUG);
	
	strcpy(str, "Usable regions: ");
	bool first = true;
	for (uint_fast8_t i = 0; i != usableIdx; i++) {
		if (!first) {strcat(str, ", ");}
		char tmp[8];
		sprintf(tmp, "%d", usableRegions[i]);
		strcat(str, tmp);
		first = false;
	}
	log(MODNAME, str, LOGLEVEL_DEBUG);
}