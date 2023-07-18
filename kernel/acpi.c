#include <stdio.h>
#include <string.h>
#include <kernel/acpi.h>
#include <kernel/panic.h>
MODULE("X86ACPI");

static bool doChecksum(ACPISDTHeader *tableHeader) {
	uint_fast8_t sum = 0;

	for (uint32_t i = 0; i < tableHeader->length; i++) {
		sum += ((char *)tableHeader)[i];
	}

	return sum == 0;
}

void ACPI_Init() {
	for (uint8_t *p = (uint8_t *)0x000E0000; p != (uint8_t *)0x000FFFFF; p++) {
		if (*p != 'R') {
			continue;
		}
		// We found an ASCII 'R', but is it correct?
		if (memcmp(p, "RSD PTR ", 8) == 0) {
			char str[64];
			sprintf(str, "Found ACPI Tables at address %p!", p);
			log(MODNAME, str, LOGLEVEL_INFO);

			RSDPDescriptor20 *rsdp = (RSDPDescriptor20 *)p;
			uint8_t rev = rsdp->firstPart.revision;

			if (rev == 0) {
				log(MODNAME, "ACPI Revision 1.0", LOGLEVEL_INFO);
			}
			else {
				log(MODNAME, "ACPI Revision >= 2.0", LOGLEVEL_INFO);
			}
			char OEMID[7] = {0};
			memcpy(OEMID, rsdp->firstPart.OEMId, 6);

			sprintf(str, "OEM ID: %s", OEMID);
			log(MODNAME, str, LOGLEVEL_INFO);


			RSDT *rsdt = (RSDT *)(uint64_t)rsdp->firstPart.RSDTAddress;
			if (!doChecksum(&rsdt->h)) {
				DUMPREGS;
				log(MODNAME, "ACPI RSDT Checksum invalid!  Uh oh....", LOGLEVEL_FATAL);
				panic("Invalid ACPI RSDT Checksum", regs);
			}
			log(MODNAME, "ACPI RSDT Checksum passed.", LOGLEVEL_INFO);

			break;
		}
	}
	while(true) {};
}