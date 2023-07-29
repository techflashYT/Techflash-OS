#include <external/limine.h>
#include <stdbool.h>
#include <stdio.h>
#include <kernel/COM.h>
#include <kernel/tty.h>
#include <kernel/arch.h>

struct limine_smp_request smpRequest = {
	.id = LIMINE_SMP_REQUEST,
	.revision = 0,
	.flags = 1
};

void main() {
	TTY_SetWriteFunc(COM_LogWrapper);
	printf(
		"SMP Request info:\r\n"
		"  - Number of CPUs: %lu\r\n"
		"  - Bootstrap Processor Local APIC ID: %lu\r\n"
		"  - X2APIC: %s\r\n",
		smpRequest.response->cpu_count,
		smpRequest.response->bsp_lapic_id,
		(smpRequest.response->flags & 1) ? "Enabled" : "Disabled"
	);
	while (true) {}
}