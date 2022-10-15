#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <kernel/shell.h>
#include <kernel/fs/tar.h>

uint8_t handleCommands(const char* input) {
	if (strcmp(input, "ver") == 0) {
		printf("Techflash OS v%u.%u.%u", CONFIG_KERN_VERSION_MAJOR, CONFIG_KERN_VERSION_MINOR, CONFIG_KERN_VERSION_PATCH);
		return 0;
	}
	if (strcmp(input, "ls") == 0) {
		ls();
		return 0;
	}
	else {
		return ERR_UNK_CMD;
	}
}