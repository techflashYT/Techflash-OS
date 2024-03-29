#include <external/limine.h>
#include <kernel/mem.h>
#include <kernel/panic.h>
#include <kernel/registers.h>

MODULE("LMN_MMAP");

volatile struct limine_memmap_request memmapRequest = {
	.id = LIMINE_MEMMAP_REQUEST,
	.revision = 0
};
volatile struct limine_hhdm_request hhdmRequest = {
	.id = LIMINE_HHDM_REQUEST,
	.revision = 0
};




static uint8_t __memmap[1024];
memmap_t *LM_ParseMemmap() {
	struct limine_hhdm_response *hhdm = hhdmRequest.response;
	struct limine_memmap_response *mmapResp = memmapRequest.response;
	if (hhdm == NULL) {
		log("No idea where the HHDM is (hhdm == NULL)!", LOGLEVEL_ERROR);
	}

	__memmap[1022] = 0xDE;
	__memmap[1023] = 0xAD;
	memmap_t *memmap = (memmap_t *)__memmap;
	if (mmapResp == NULL) {
		registers_t regs;
		DUMPREGS(&regs);
		panic("Memory map response from Limine was NULL.  Not continuing with bogus map!", &regs);
	}
	for (uint_fast8_t i = 0; i != mmapResp->entry_count; i++) {
		struct limine_memmap_entry *cur = mmapResp->entries[i];

		memmap->numEntries++;
		memmap->entries[i].start = (void *)cur->base;
		memmap->entries[i].size  = cur->length;

		     if (cur->type == LIMINE_MEMMAP_USABLE)                 {memmap->entries[i].type = MM_TYPE_FREE;}
		else if (cur->type == LIMINE_MEMMAP_RESERVED)               {memmap->entries[i].type = MM_TYPE_RSRV;}
		else if (cur->type == LIMINE_MEMMAP_ACPI_RECLAIMABLE)       {memmap->entries[i].type = MM_TYPE_ACPI; memmap->entries[i].flags = MM_FLAG_RECLAIM; }
		else if (cur->type == LIMINE_MEMMAP_ACPI_NVS)               {memmap->entries[i].type = MM_TYPE_ACPI; memmap->entries[i].flags = MM_FLAG_READONLY;}
		else if (cur->type == LIMINE_MEMMAP_BAD_MEMORY)             {memmap->entries[i].type = MM_TYPE_RSRV; memmap->entries[i].flags = MM_FLAG_BAD_MEM; }
		else if (cur->type == LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE) {memmap->entries[i].type = MM_TYPE_BOOT; memmap->entries[i].flags = MM_FLAG_RECLAIM; }
		else if (cur->type == LIMINE_MEMMAP_KERNEL_AND_MODULES)     {memmap->entries[i].type = MM_TYPE_BOOT; memmap->entries[i].flags = MM_FLAG_KERNEL | MM_FLAG_MODULES;}
		else if (cur->type == LIMINE_MEMMAP_FRAMEBUFFER)            {memmap->entries[i].type = MM_TYPE_MMIO; memmap->entries[i].flags = MM_FLAG_FRAMEBUF;}
		else                                                        {memmap->entries[i].type = MM_TYPE_UNKN;}
	}
	if (__memmap[1022] != 0xDE || __memmap[1023] != 0xAD) {
		registers_t regs;
		DUMPREGS(&regs);

		panic("PMM: __memmap trashed", &regs);
	}
	return memmap;
}