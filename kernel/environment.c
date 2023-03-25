#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <kernel/environment.h>
MODULE("KERN_ENV");
static void parse(uint16_t size);

void handleEnv() {
	uint16_t i = 0;
	log(MODNAME, "Printing entire env!", LOGLEVEL_VERBOSE);
	
	while (environment[i] != '\0') {
		char byte = environment[i];
		if (byte == '\n') {
			putchar('\r');
		}
		putchar(byte);
		i++;
	}
	puts("\r\n");
	char *buffer = malloc(35);
	strcpy(buffer, "Entire env printed, ");
	utoa(i, buffer + strlen(buffer), 10);
	strcat(buffer, " bytes.");
	log(MODNAME, buffer, LOGLEVEL_VERBOSE);
	free(buffer);
	parse(i);
	return;
}
static void parse(uint16_t size) {
	log(MODNAME, "Started parsing the environment variables.", LOGLEVEL_VERBOSE);
	uint16_t i = 0;
	bool workingOn;
	char *currentKey = malloc(51);
	char *currentVal = malloc(51);
	while (true) {
		break;
		uint8_t currentKeyOrValIndex = 0;
		if (i >= size) {
			goto leave;
		}
		for (;; i++, currentKeyOrValIndex++) {
			if (workingOn) { // key
				if (environment[i] == '=') {
					workingOn = false;
					goto end;
				}
				currentKey[currentKeyOrValIndex] = environment[i];
				goto end;
			}
			// value
			if (environment[i] == ' ') {
				workingOn = true;
				goto end;
			}
			currentVal[currentKeyOrValIndex] = environment[i];
		end:
		}
	}
leave:
	free(currentKey);
	free(currentVal);
	log(MODNAME, "Finished parsing the environment variables.", LOGLEVEL_VERBOSE);
	return;
}
