#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024
#define MAX_NUM_SYMS 1000

int main() {
	// Run `nm bin/tfos_kernel.elf` and get output into array of strings
	char (*nmOutput)[MAX_LINE_LENGTH] = malloc(sizeof(char[MAX_NUM_SYMS][MAX_LINE_LENGTH]));
	FILE *nmFp = popen("nm build/tfos_kernel_nosyms.elf", "r");
	if (!nmFp) {
		perror("Failed to run nm command");
		exit(1);
	}
	uint16_t numSyms = 0;
	while (numSyms < MAX_NUM_SYMS && fgets(nmOutput[numSyms], MAX_LINE_LENGTH, nmFp)) {
		numSyms++;
	}
	pclose(nmFp);

	// Write header to file
	FILE *symtabFp = fopen("build/symbols.bin", "w");
	if (!symtabFp) {
		perror("Failed to open output file");
		exit(1);
	}
	uint8_t magic[4] = {'S', 'Y', 'M', 'T'};
	uint8_t flagsReserved = 0;
	
	fwrite(magic, 4, 1, symtabFp);
	fwrite(&flagsReserved, 1, 1, symtabFp);
	fwrite(&numSyms, 2, 1, symtabFp);

	// Loop over every line of nm output, if the 18th character is `T`, it's a global func/var, add it to the list in the file
	for (int i = 0; i < numSyms; i++) {
		if (nmOutput[i][17] = 'T') {
			// Parse address and name from the nm output line
			char *addrStr = strtok(nmOutput[i], " ");
			char *name = strtok(NULL, " ");
			uint64_t addr = strtoull(addrStr, NULL, 16);

			// Search for newline or null terminator starting from the 30th character to get the symbol name
			char *nameStart = &nmOutput[i][19];
			char *nameEnd = strchr(nameStart, '\n');
			if (!nameEnd) {
				nameEnd = strchr(nameStart, '\0');
			}

			// Calculate the length of the symbol name
			size_t nameLen = nameEnd - nameStart;

			// For every global function/variable, write it to the file
			fwrite(&addr, 8, 1, symtabFp);
			fwrite(&nameLen, sizeof(nameLen), 1, symtabFp);
			fwrite(nameStart, nameLen, 1, symtabFp);
		}
	}


	fclose(symtabFp);

	// Free allocated memory
	free(nmOutput);

	return 0;
}