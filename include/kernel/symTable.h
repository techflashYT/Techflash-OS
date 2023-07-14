#include <stdint.h>
// S, Y, M, T
extern char symTableMagic[4];

typedef struct __attribute__((packed)) {
	char magic[4];
	// reserved
	uint8_t flags;
	uint16_t numSyms;
} __attribute__((packed)) symTable_t;
extern symTable_t symTable;
typedef struct __attribute__((packed)) {
	void *address;
	// null terminated
	char name[];
} __attribute__((packed)) symTableEntry_t;

typedef struct {
    char *name;
    uint64_t offset;
} symbolConvInfo_t;
extern volatile symbolConvInfo_t *getSymbolByAddress(uint64_t address, volatile symbolConvInfo_t *info);