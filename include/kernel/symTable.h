#include <stdint.h>
// S, Y, M, T
extern char symTableMagic[4];
typedef struct {
	char magic[4];
	// reserved
	uint8_t flags;
	uint16_t numSyms;
	struct {
		uint64_t address;
		uint8_t nameLen;
		// NOT null terminated!!!  Use nameLen.
		char name[];
	} symbolEntries[];
} symTable_t;

typedef struct {
	char *name;
	uint16_t offset;
} symbolConvInfo_t;