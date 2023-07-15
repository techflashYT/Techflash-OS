#include <kernel/elf.h>
#include <kernel/arch.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <kernel/custom.h>
elfLoader_t elfLoader;
elfStruct_t retVal;
static uint8_t load(ELF64ProgramHeader *ph);
MODULE("ELFLOAD");
elfStruct_t *elfLoad(uint8_t *file) {
	// NOTE: This function 100% assumes that everything about the ELF is valid
	//       We expect the caller to have already ran elfLoader.isValid() on the ELF
	//       in order to verify it's validity before calling elfLoader.load() to run this.
	log(MODNAME, "Starting to load ELF file", LOGLEVEL_DEBUG);
	retVal.entryPointOffset = 0;
	retVal.startOfData = 0;
	// Map the header to the start of the file
	ELF64FileHeader *header;
	header = (ELF64FileHeader *)file;
	uint16_t phNum = 0;
	ELF64ProgramHeader *ph = NULL;
	for (uint64_t pos = header->phTablePos; phNum != header->phtableEntriesNum; pos += header->phTableEntrySize) {
		phNum++;
		ph = (ELF64ProgramHeader *)(file + pos);
		switch (ph->segmentType) {
			case PH_TYPE_INTERPRETER: {
				log(MODNAME, "This ELF requires an interpreter!  We haven't implemented that yet.", LOGLEVEL_ERROR);
				retVal.err = 1;
				return &retVal;
				break;
			}
			case PH_TYPE_DYNAMIC: {
				log(MODNAME, "This ELF is dynamically linked!  We haven't implemented that yet.", LOGLEVEL_ERROR);
				retVal.err = 2;
				return &retVal;
				break;
			}
			case PH_TYPE_NOTE: {
				log(MODNAME, "Note section, ignoring.", LOGLEVEL_DEBUG);
				break;
			}
			case PH_TYPE_LOAD: {
				// the juicy stuff, this is some code/data right here.
				uint8_t err = load(ph);
				if (err != 0) {
					retVal.err = err + 2;
					return &retVal;
				}
				break;
			}
			default: {
				log(MODNAME, "Unknown section, ignoring.", LOGLEVEL_DEBUG);
				// something else, very likely something we don't care about, skip
				break;
			}
		}
	}
	free(ph);
	retVal.startOfData = 0;
	retVal.entryPointOffset = header->entryPoint;
	// TODO: Set up a stack
	return &retVal;
}
static uint8_t load(ELF64ProgramHeader *ph) {
	// we're just gonna use the memory it wants and hope that it's free

	// copy it into memory
	// a couple of variables, since we're gonna be logging this info before we use it, no sense calculating it twice!
	void *srcAddr = (ph + ph->p_offset);
	void *destAddr = (void *)(ph->p_vaddr);
	uint64_t size = ph->p_filesz;
	{
		char buffer[128];

		sprintf(buffer,
			"%s %d bytes into memory from %p to %p%s",
			(size == 0) ? "Not bothering to copy" : "Loading",
			size, srcAddr, destAddr,
			(size == 0) ? ", since there's nothing to copy!" : ".  This could cause issues."
		);
		log(MODNAME, buffer, LOGLEVEL_WARN);
	}
	
	if (size != 0) {
		memcpy(destAddr, srcAddr, size);
	}
	return 0;
}