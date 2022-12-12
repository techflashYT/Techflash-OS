#include <kernel/elf.h>
#include <kernel/log.h>
#include <kernel/arch.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define MIN2(a, b) ((a) < (b) ? (a) : (b))
#define MAX2(a, b) ((a) > (b) ? (a) : (b)) // GUESSING

elfStruct_t retVal;
ElfHeader header;
ElfProgramHeader ph;
elfStruct_t *elfLoad(uint8_t *file, size_t fileSize) {
	int i;
	uintptr_t base = UINTPTR_MAX;

	memcpy(&header, file, sizeof(header));

	if (elfLoader.isValid(file, ARCH_X86_64) != 0) {
		retVal.entryPointOffset = 0;
		retVal.startOfData = 0;
		return &retVal;
	}

	/*note that we copy each program header into ph
	  to avoid any alignment errors
	*/

	for (i = 0; i < header.e_phnum; i++) {
		memcpy(file + header.e_phoff + header.e_phentsize*i, &ph, sizeof(ph));

		base = MIN2(base, ph.p_vaddr);
	}

	uintptr_t size = 0;

	for (i = 0; i < header.e_phnum; i++) {
		uintptr_t segmentEnd;

		memcpy(file + header.e_phoff + header.e_phentsize*i, &ph, sizeof(ph));

		segmentEnd = ph.p_vaddr - base + ph.p_memsz;
		size = MAX2(size, segmentEnd);
	}
	// uint8_t *finalImage = malloc(size); // for PIC
	uint8_t *finalImage = malloc(fileSize);
	// unsigned char *finalImage = (unsigned char*) base; // for absolute

	for (i = 0; i < header.e_phnum; i++) {
		uintptr_t addr;

		memcpy(file + header.e_phoff + header.e_phentsize * i, &ph, sizeof(ph));

		addr = (ph.p_vaddr - base) + (uintptr_t)finalImage;

		memset(finalImage + addr, 0, ph.p_memsz);
		memcpy(finalImage + addr, file + ph.p_offset, ph.p_filesz);
	}
	memcpy(finalImage, file, fileSize);
	retVal.startOfData = finalImage;
	retVal.entryPointOffset = header.e_entry;
	return &retVal; 
}