#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <kernel/elf.h>
const char elfMagic[] = {EIMAG0, EIMAG1, EIMAG2, EIMAG3};
const uint8_t archConvert[] = {
	elfArch_x86_64,
	elfArch_i386,
	elfArch_arm32,
	elfArch_arm64
};
const char *archConvertStr[] = {
	"x86_64",
	"i386",
	"ARM32",
	"ARM64"
};
// please see elfLoader.isValid() for docs (elf.h)
uint8_t elfCheckIsValid(uint8_t *file, uint8_t arch) {
	if (memcmp(file, elfMagic, 4) != 0) {
		// wtf?  This isn't an elf...
		log("ELFLOAD", "File does not have ELF magic (0x7F, 'E', 'L', 'F').", LOGLEVEL_ERROR);
		log("ELFLOAD", "Either this file is not an ELF at all (likely), or it's heavily corrupted.", LOGLEVEL_DEBUG);
		return 1;
	}
	// Ok, it's a valid ELF, but is it correct for this arch?
	ELF64FileHeader *header;
	header = (ELF64FileHeader *)file;
	if (header->arch != archConvert[arch]) {
		// Nope, log and return 2
		log("ELFLOAD", "File is a valid ELF, but is not valid for this CPU architecture.", LOGLEVEL_ERROR);
		char *buffer = malloc(128);
		strcpy(buffer, "You are running the ");
		char *currentPtr = buffer + strlen(buffer);
		strcpy(currentPtr, archConvertStr[arch]);
		currentPtr = buffer + strlen(buffer);
		strcpy(currentPtr, " version of Techflash OS.  However, this ELF file is only for ");
		currentPtr = buffer + strlen(buffer);
		if (header->arch == elfArch_i386)         strcpy(currentPtr, "i386");
		else if (header->arch == elfArch_x86_64)  strcpy(currentPtr, "x86_64");
		else if (header->arch == elfArch_arm32)   strcpy(currentPtr, "ARM32");
		else if (header->arch == elfArch_arm64)   strcpy(currentPtr, "ARM64");
		else if (header->arch == elfArch_powerpc) strcpy(currentPtr, "PowerPC");
		else if (header->arch == elfArch_sparc)   strcpy(currentPtr, "SPARC");
		else if (header->arch == elfArch_mips)    strcpy(currentPtr, "MIPS");
		else if (header->arch == elfArch_ia64)    strcpy(currentPtr, "IA-64");
		else if (header->arch == elfArch_riscv)   strcpy(currentPtr, "RISC-V");
		else if (header->arch == elfArch_superh)  strcpy(currentPtr, "SuperH");
		else if (header->arch == elfArch_none)    strcpy(currentPtr, "None");
		else                                      strcpy(currentPtr, "*Invalid*");
		currentPtr = buffer + strlen(buffer);
				
		
		strcpy(currentPtr, " CPUs.");

		log("ELFLOAD", buffer, LOGLEVEL_DEBUG);
		free(buffer);
		return 2;
	}

	return 0;
}