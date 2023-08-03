#include <external/limine.h>
#include <kernel/mem.h>
#include <kernel/bootloader.h>
#include <kernel/registers.h>
#include <kernel/panic.h>
#include <stddef.h>
#include <stdio.h>
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
void PMM_Init() {
	log(MODNAME, "PMM Initializing", LOGLEVEL_INFO);
	log(MODNAME, "Getting memory map from bootloader...", LOGLEVEL_DEBUG);
	log(MODNAME, "====== Memory Map ======", LOGLEVEL_DEBUG);
	char str[128];

	memmap_t *memmap = BOOT_ParseMemmap();

	sprintf(str, "Got memory map with %d entries at %p", memmap->numEntries, memmap);
	log(MODNAME, str, LOGLEVEL_DEBUG);
}