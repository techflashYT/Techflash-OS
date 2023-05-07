#include <kernel/symTable.h>

symTable_t symTable = {
	{'N', 'O', 'P', 'E'},
	0xFF,
	0,
	.symbolEntries = {{
		.address = 0xDEADBEEF,
		// .name = {'Y', 'o', 'u', 'r', ' ', 'm', 'o', 'm'}
	}}
};