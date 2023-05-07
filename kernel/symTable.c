#include <kernel/symTable.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
char symTableMagic[4] = {'S', 'Y', 'M', 'T'};
bool validSymTable = true;
#pragma GCC optimize("O0")
symbolConvInfo_t *getSymbolByAddress(uint64_t address, symbolConvInfo_t *info) {
	if (validSymTable) {
		if (!(memcmp(symTableMagic, symTable.magic, 4) == 0)) {
			log("SYMTABLE", "Symbol Table magic ('SYMT') invalid.  Giving up on symbols.", LOGLEVEL_ERROR);
			validSymTable = false;
			goto err;
		}
		
		for (uint_fast16_t i = 0; i < symTable.numSyms; i++) {
			if (address < symTable.symbolEntries[i].address) {
				// the last symbol we hit was correct
				i--;
				uint16_t offset = address - symTable.symbolEntries[i].address;
				info->offset = offset;
				info->name = symTable.symbolEntries[i].name;
			}
		}
		return info;
	}
	err:
		return NULL;
}