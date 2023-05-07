#include <kernel/symTable.h>

symTable_t symTable = {
	{'S', 'Y', 'M', 'T'},
	0,
	1,
	.symbolEntries = {{
		.address = 0xDEADBEEF,
		.nameLen = 0
		// .name = {'Y', 'o', 'u', 'r', ' ', 'm', 'o', 'm'}
	}}
};