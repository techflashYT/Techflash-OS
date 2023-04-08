#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <kernel/shell.h>
#include <kernel/fs/tar.h>
#include <kernel/tty.h>
#include <kernel/environment.h>
#include <kernel/hardware/serial.h>
extern void CPUReset();
char *arguments;
typedef struct {
	const char *command;
	const char *description;
} helpDBentry_t;
typedef struct {
	helpDBentry_t entries[100];
	uint_fast8_t maxCMDNameLen;
} helpDB_t;
helpDB_t helpDB = {{
	{"help",        "You're reading it right now!"},
	{"clear",       "Clears the screen"},
	{"ls",          "Lists the files in the specified directory, if none is specified, list the current directory"},
	{"ver",         "Prints the version of Techflash OS that you are running"},
	{"reboot",      "Reboots the system"},
	{"serialwrite", "Usage: serialwrite *port* *msg*.  Write a string down the serial port specified"},
	{"",            ""}}, // indicates the end of the helpDB
	.maxCMDNameLen = 11
};
void help() {
	if (arguments[0] == '\0') {
		puts("Commands:\r\n");
		// list the entire helpDB
		for (uint_fast8_t i = 0; helpDB.entries[i].command[0] != 0; i++) {
			puts("\t- `");
			puts(helpDB.entries[i].command);
			puts("`: ");
			for (uint_fast8_t j = 0; j != (helpDB.maxCMDNameLen - strlen(helpDB.entries[i].command)); j++) {
				putchar(' ');
			}
			puts(helpDB.entries[i].description);
			puts("\r\n");
		}
		return;
	}
	// get the first arg
	char *firstArg = malloc(64);
	uint_fast8_t i = 0;
	while (arguments[i] != '\0' && arguments[i] != ' ') {
		firstArg[i] = arguments[i];
		i++;
	}
	// iterate through the helpDB until we find the command from the argument
	bool found = false;
	for (uint_fast8_t i = 0; helpDB.entries[i].command[0] != 0; i++) {
		if (strcmp(firstArg, helpDB.entries[i].command) == 0) {
			// we found a match, print it's description
			found = true;
			putchar('`');
			puts(helpDB.entries[i].command);
			puts("`: ");
			puts(helpDB.entries[i].description);
			puts("\r\n");
		}
	}
	if (!found) {
		puts("No match for `");
		puts(firstArg);
		puts("`.\r\n");
	}
	free(firstArg);
	return;
}
bool checkInput(const char* str1, const char* str2) {
	for (uint_fast16_t i = 0; i < (uint_fast16_t)-1; i++) {
		if (str1[i] == str2[i]) {
			if (str1[i] == '\0' && str2[i] == '\0') {
				return true;
			}
			continue;
		}
		else if (str1[i] == ' ') {
			strcpy(arguments, (str1 + i + 1));
			return true;
		}
		else if (str1[i] == '\0') {
			return false;
		}
		else if (str1[i] != str2[i]) {
			return false;
		}
	}
	// unreachable
	return false;
}
uint_fast8_t handleCommands(const char* input) {
	memset(arguments, '\0', 128);
	if (input[0] == '\0') {
		return 0;
	}
	else if (checkInput(input, "ver")) {
		printf("Techflash OS v%u.%u.%u\r\n", CONFIG_KERN_VERSION_MAJOR, CONFIG_KERN_VERSION_MINOR, CONFIG_KERN_VERSION_PATCH);
		return 0;
	}
	else if (checkInput(input, "help")) {
		help();
		return 0;
	}
	else if (checkInput(input, "ls")) {
		ls(arguments);
		return 0;
	}
	else if (checkInput(input, "reboot")) {
		CPUReset();
		return 0;
	}
	else if (checkInput(input, "scroll")) {
		TTY_Scroll(arguments);
		return 0;
	}
	else if (checkInput(input, "clear")) {
		memset(&fb, 0, bootboot.fb_size);
		TTY_CursorX = 0;
		TTY_CursorY = 0;
		TTY_CursorAfterPromptX = 0;
		return 0;
	}
	else if (checkInput(input, "serialwrite")) {
		// get the first arg
		char *firstArg  = malloc(64);
		firstArg[0] = '\0';
		char *secondArg = malloc(256);
		secondArg[0] = '\0';
		uint_fast8_t i = 0;
		uint_fast8_t j = 0;
		while (arguments[i] != '\0') {
			if (arguments[i] == ' ') {
				i++;
				break;
			}
			firstArg[i] = arguments[i];
			i++;
		}
		if (firstArg[0] == '\0') {
			// no args, give up
			puts("Usage: serialwrite *port* *message*\r\n");
			goto end;
		}
		while (arguments[i] != '\0') {
			secondArg[j] = arguments[i];
			j++;
			i++;
		}
		if (secondArg[0] == '\0') {
			// only 1 arg, give up
			puts("Usage: serialwrite *port* *message*\r\n");
			goto end;
		}
		uint16_t convert[] = {
			SERIAL_PORT_COM1,
			SERIAL_PORT_COM2,
			SERIAL_PORT_COM3,
			SERIAL_PORT_COM4,
			SERIAL_PORT_COM5,
			SERIAL_PORT_COM6,
			SERIAL_PORT_COM7,
			SERIAL_PORT_COM8
		};
		uint_fast16_t port = atoi(firstArg);
		if (port == 0) {
			puts("ERROR: Port cannot be < 1");
			goto end;
		}
		if (port > 8) {
			puts("ERROR: Port cannot be > 8.");
			goto end;
		}
		
		serial.writeString(convert[port - 1], secondArg);

		end:
			free(firstArg);
			free(secondArg);
			return 0;
	}
	return ERR_UNK_CMD;
}