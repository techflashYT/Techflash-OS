#include <external/limine.h>
#include <kernel/panic.h>
#include <kernel/registers.h>
#include <stddef.h>
volatile struct limine_hhdm_request memmapRequest = {
	.id = LIMINE_HHDM_REQUEST,
	.revision = 0
};

void PMM_Init() {
	if (memmapRequest.response == NULL) {
		registers_t regs;
		DUMPREGS(&regs);
		panic("Memory map response from Limine was NULL.  Not continuing with bogus map!", &regs);
	}
}