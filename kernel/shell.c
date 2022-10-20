#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <kernel/shell.h>
#include <kernel/fs/tar.h>
char *arguments;
bool checkInput(const char* str1, const char* str2) {
	for (uint16_t i = 0; i < (uint16_t)-1; i++) {
		if (str1[i] == str2[i]) {
			if (str1[i] == '\0' && str2[i] == '\0') {
				return true;
			}
			continue;
		}
		else if (str1[i] == ' ') {
			strcpy(arguments, (str1 + i));
			return true;
		}
		else if (str1[i] == '\0') {
			return false;
		}
		else if (str1[i] != str2[i]) {
			return false;
		}
	}
}
uint8_t handleCommands(const char* input) {
	if (checkInput(input, "ver")) {
		printf("Techflash OS v%u.%u.%u\r\n", CONFIG_KERN_VERSION_MAJOR, CONFIG_KERN_VERSION_MINOR, CONFIG_KERN_VERSION_PATCH);
		return 0;
	}
	else if (checkInput(input, "help")) {
		puts (
"\
Commands:\r\n\
	- `help`: You're reading it right now!\r\n\
	- `ls`:   Lists the files in the directory\r\n\
	- `ver`:  Prints the version of Techflash OS that you are running\r\n\
"
		);
		return 0;
	}
	else if (checkInput(input, "ls")) {
		ls(arguments);
		return 0;
	}
	else if (checkInput(input, "reboot")) {
		CPUReset();
	}
	return ERR_UNK_CMD;
}