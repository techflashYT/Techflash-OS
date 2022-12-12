#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
typedef struct {
	uint8_t *startOfData;
	uint64_t entryPointOffset;
} elfStruct_t;

typedef struct {
	uint8_t (*isValid)(uint8_t *header, uint8_t arch);
	elfStruct_t *(*load)(uint8_t *file, size_t fileSize);
} elfLoader_t;
extern elfLoader_t elfLoader;
#define EI_NIDENT	16
typedef struct ElfHeader{
	unsigned char e_ident[EI_NIDENT]; //should start with [0x7f 'E' 'L' 'F']
	uint16_t e_type;
	uint16_t e_machine;
	uint32_t e_version;
	uint32_t e_entry;
	uint32_t e_phoff;     //start of program headers in file
	uint32_t e_shoff;
	uint32_t e_flags;
	uint16_t e_ehsize;
	uint16_t e_phentsize; //size of each program header
	uint16_t e_phnum;     //number of program headers
	uint16_t e_shentsize;
	uint16_t e_shnum;
	uint16_t e_shstrndx;
} ElfHeader;

typedef struct ElfProgramHeader {
	uint32_t p_type;
	uint32_t p_offset; //offset of data in elf image
	uint32_t p_vaddr;  //virtual load address
	uint32_t p_paddr;  //physical load address, not used
	uint32_t p_filesz; //size of data in elf image
	uint32_t p_memsz;  //size of data in memory; any excess over disk size is zero'd
	uint32_t p_flags;
	uint32_t p_align;  //alignment
} ElfProgramHeader;

enum {
	PT_NULL,
	PT_LOAD,
	PT_DYNAMIC,
	PT_INTERP,
	PT_NOTE,
	PT_SHLIB,
	PT_PHDR,
	PT_LOPROC = 0x70000000, //reserved
	PT_HIPROC = 0x7FFFFFFF  //reserved
};