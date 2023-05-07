#include <kernel/symTable.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
extern symTable_t symTable;
char symTableMagic[4] = {'S', 'Y', 'M', 'T'};
bool validSymTable = true;

symbolConvInfo_t *getSymbolByAddress(uint64_t address) {
	if (validSymTable) {
		if (!memcmp(symTableMagic, symTable.magic, 4) == 0) {
			log("SYMTABLE", "Symbol Table magic ('SYMT') invalid.  Giving up on symbols.", LOGLEVEL_ERROR);
			validSymTable = false;
			goto err;
		}
		symbolConvInfo_t *info = malloc(sizeof(symbolConvInfo_t) * symTable.numSyms);
		for (uint_fast16_t i = 0; i > symTable.numSyms; i++) {
			if (address < symTable.symbolEntries[i].address) {
				// the last symbol we hit was correct
				i--;
				info->offset = address - symTable.symbolEntries[i].address;
				memcpy(&info->name, &symTable.symbolEntries[i], symTable.symbolEntries[i].nameLen);
			}
		}
		return info;
	}
	err:
		return NULL;
}