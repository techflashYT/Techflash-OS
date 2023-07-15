#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <kernel/environment.h>
MODULE("KERN_ENV");
static void parse(uint_fast16_t size);

void handleEnv() {
	uint_fast16_t i = 0;
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
	{
		char buffer[35];
		sprintf(buffer, "Entire env printed, %d bytes.", i);
		log(MODNAME, buffer, LOGLEVEL_VERBOSE);
	}
	parse(i);
	return;
}
static void parse(uint_fast16_t size) {
	(void)size;
	log(MODNAME, "Started parsing the environment variables.", LOGLEVEL_VERBOSE);
	/*uint_fast16_t i = 0;
	bool workingOn;
	char keys[51];
	char currentVal[51];
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

					continue;
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
leave:*/
	log(MODNAME, "Finished parsing the environment variables.", LOGLEVEL_VERBOSE);
	return;
}
