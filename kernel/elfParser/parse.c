#include <kernel/elf.h>
#include <kernel/log.h>
#include <kernel/arch.h>
#include <string.h>
#include <stdio.h>
#define MIN2(a, b) ((a) < (b) ? (a) : (b))

int elfLoad(uint8_t *file) {
	ElfHeader header;
	ElfProgramHeader ph;
	int i;
	uintptr_t base = UINTPTR_MAX;

	memcpy(&header, file, sizeof(header));

	elfLoader.isValid(&header, ARCH_X86_64);

	/*note that we copy each program header into ph
	  to avoid any alignment errors
	*/

	for (i = 0; i<header.e_phnum; i++) {
		memcpy(file + header.e_phoff + header.e_phentsize*i, &ph, sizeof(ph));

		base = MIN2(base, ph.p_vaddr);
	}

	return 0;
}