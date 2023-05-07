#include <stdint.h>
// S, Y, M, T
extern char symTableMagic[4];
typedef struct __attribute__((packed)) {
	char magic[4];
	// reserved
	uint8_t flags;
	uint16_t numSyms;
	struct __attribute__((packed)) {
		uint64_t address;
		// null terminated
		char name[];
	} __attribute__((packed)) symbolEntries[];
} __attribute__((packed)) symTable_t;
extern symTable_t symTable;

typedef struct {
	char *name;
	uint16_t offset;
} symbolConvInfo_t;
symbolConvInfo_t *getSymbolByAddress(uint64_t address, symbolConvInfo_t *info);