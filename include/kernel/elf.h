#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
typedef struct {
	bool err;
	uint8_t *startOfData;
	uint64_t entryPointOffset;
} elfStruct_t;
#define EIMAG0 0x7F
#define EIMAG1 'E'
#define EIMAG2 'L'
#define EIMAG3 'F'
typedef struct {
	uint8_t  magic[3];
	uint8_t  bits;
	uint8_t  endianness;
	uint8_t  headerVersion;
	uint8_t  abi;
	uint64_t padding1;
	uint16_t type;
	uint16_t arch;
	uint32_t ELFVersion;
	uint64_t entryPoint;
	uint64_t phTablePos;
	uint64_t sectionHeaderTablePos;
	uint32_t flags;
	uint16_t headerSize;
	uint16_t phTableEntrySize;
	uint16_t phtableEntriesNum;
	uint16_t sectionHeaderTableEntrySize;
	uint16_t sectionHeaderTableNamesIndex;
} ELF64FileHeader;

#define elfArch_none    0
#define elfArch_sparc   2
#define elfArch_i386    3
#define elfArch_mips    8
#define elfArch_powerpc 0x14
#define elfArch_arm32   0x28
#define elfArch_superh  0x2A
#define elfArch_ia64    0x32
#define elfArch_x86_64  0x3E
#define elfArch_arm64   0xB7
#define elfArch_riscv   0xF3

typedef struct {
	/*
	Types:
		0: ignore
		1: load - clear `p_memsz` bytes at `p_vaddr` to 0, then copy `p_filesz` bytes from `p_offset` to `p_vaddr`
		2: dynamic - requires dynamic linking
		3: interp - requires interpreter
		4: note - note section, usually inserted by the compiler, assembler, and linker, simply ignore it
	*/
	uint32_t segmentType;
	/*
	Flags:
		1: executable
		2: writable
		4: readable
	*/
	uint32_t flags;
	uint64_t p_offset; // offset of the data in the file
	uint64_t p_vaddr;  // where should we start putting it in memory at
	uint64_t undefined;
	uint64_t p_filesz; // size of the segment in the file
	uint64_t p_memsz;  // size of the segment in memory
	uint64_t alignment;
} ELF64ProgramHeader;

#define PH_TYPE_LOAD        1
#define PH_TYPE_DYNAMIC     2
#define PH_TYPE_INTERPRETER 3
#define PH_TYPE_NOTE        4
typedef struct {
	/*
		Checks if the file is a valid ELF application that can run on this CPU.
		Returns:
		0 - Valid ELF that can run on the specified arch
		1 - Invalid ELF magic (either corrupted or simply not an ELF)
		2 - Valid ELF, but not for this CPU
		3 - Valid ELF, for this CPU too, but endianness mismatch
	*/
	uint8_t (*isValid)(uint8_t *header, uint8_t arch);
	elfStruct_t *(*load)(uint8_t *file);
} elfLoader_t;
extern elfLoader_t elfLoader;
