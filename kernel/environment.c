#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <kernel/environment.h>
#include <kernel/misc.h>
MODULE("KERN_ENV");
static void parse(uint16_t size);
environment_t handleEnv() {
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
	char *buffer = malloc(256);
	strcpy(buffer, "Entire env printed,  ");
	utoa(i, buffer + strlen(buffer), 10);
	strcpy(buffer + strlen(buffer), " bytes.");
	parse(i);
	log(MODNAME, buffer, LOGLEVEL_VERBOSE);
}
static void parse(uint16_t size) {

}