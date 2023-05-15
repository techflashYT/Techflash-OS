#include <kernel/symTable.h>
#include <kernel/custom.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
char symTableMagic[4] = {'S', 'Y', 'M', 'T'};
bool validSymTable = true;
#pragma GCC optimize("O0")
MODULE("SYMTABLE");
volatile symbolConvInfo_t *getSymbolByAddress(uint64_t address, volatile symbolConvInfo_t *info) {
	if (validSymTable) {
		if (!(memcmp(symTableMagic, symTable.magic, 4) == 0)) {
			log(MODNAME, "Symbol Table magic ('SYMT') invalid.  Giving up on symbols.", LOGLEVEL_ERROR);
			validSymTable = false;
			goto err;
		}
		symTableEntry_t *lastAddr = 0;
		symTableEntry_t *nextPtr = (symTableEntry_t *)((uint8_t *)(&symTable.numSyms) + 2);
		uint16_t i = 0;
		for (; i < symTable.numSyms;) {
			/*char *str = "addr = 0x\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
			utoa((uint_fast64_t)nextPtr, str + 9, 16);
			padTo(str + 9, 16);
			log(MODNAME, str, LOGLEVEL_WARN);*/
			if (address < nextPtr->address) {
				// the last symbol we hit was correct
				nextPtr = lastAddr;
				uint16_t offset = address - nextPtr->address;
				info->offset = offset;
				info->name = nextPtr->name;
				break;
			}
			lastAddr = nextPtr;
			// pointer stuff is weird, this might help
			void *newPtr = nextPtr + sizeof(uint64_t) + strlen(nextPtr->name) + 1;
			nextPtr = newPtr;
			nextPtr = newPtr;
		}
		return info;
	}
	err:
		return NULL;
}