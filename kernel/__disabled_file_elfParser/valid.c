#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <kernel/elf.h>
const char elfMagic[] = {EIMAG0, EIMAG1, EIMAG2, EIMAG3};
const uint_fast8_t archConvert[] = {
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
uint_fast8_t elfCheckIsValid(uint8_t *file, uint_fast8_t arch) {
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
		{
			char buffer[128];
			char ELFarch[16];
			if      (header->arch == elfArch_i386)    strcpy(ELFarch, "i386");
			else if (header->arch == elfArch_x86_64)  strcpy(ELFarch, "x86_64");
			else if (header->arch == elfArch_arm32)   strcpy(ELFarch, "ARM32");
			else if (header->arch == elfArch_arm64)   strcpy(ELFarch, "ARM64");
			else if (header->arch == elfArch_powerpc) strcpy(ELFarch, "PowerPC");
			else if (header->arch == elfArch_sparc)   strcpy(ELFarch, "SPARC");
			else if (header->arch == elfArch_mips)    strcpy(ELFarch, "MIPS");
			else if (header->arch == elfArch_ia64)    strcpy(ELFarch, "IA-64");
			else if (header->arch == elfArch_riscv)   strcpy(ELFarch, "RISC-V");
			else if (header->arch == elfArch_superh)  strcpy(ELFarch, "SuperH");
			else if (header->arch == elfArch_none)    strcpy(ELFarch, "None");
			else                                      strcpy(ELFarch, "*Invalid*");
			sprintf(buffer, "You are running the %s version of Techflash OS.  However, this ELF file is only for %s CPUs.", archConvertStr[arch], ELFarch);
			
			log("ELFLOAD", buffer, LOGLEVEL_DEBUG);
		}
		return 2;
	}

	return 0;
}