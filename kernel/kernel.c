#include <external/limine.h>
#include <stdbool.h>
#include <stdio.h>
#include <kernel/COM.h>
#include <kernel/tty.h>
#include <kernel/arch.h>

volatile struct limine_smp_request smpRequest = {
	.id = LIMINE_SMP_REQUEST,
	.revision = 0,
	.flags = 0
};

void main() {
	TTY_SetWriteFunc(COM_LogWrapper);
	if (smpRequest.response == NULL) {
		puts("Limine didn't give us a valid SMP response...\r\n");
	}
	printf(
		"SMP Response Info:\r\n"
		"  - Response revision: %lu\r\n"
		"  - Number of CPUs: %lu\r\n"
		"  - Bootstrap Processor Local APIC ID: %u\r\n"
		"  - X2APIC: %s\r\n",
		smpRequest.response->revision,
		smpRequest.response->cpu_count,
		smpRequest.response->bsp_lapic_id,
		(smpRequest.response->flags & 1) ? "Enabled" : "Disabled"
	);
	while (true) {}
}