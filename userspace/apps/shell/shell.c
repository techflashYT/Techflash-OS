#include <stdio.h>
#include <string.h>

#include <userspace/apps/shell/shell.h>
#include <kernel/tty.h>
// A super basic terminal
char prompt[32];
void handleCommand(const char* command) {
	if (strcmp(command, "help") == 0) {
		printf("help\r\n");
	}
	else if (strcmp(command, "clear") == 0) {
		terminalClear();
	}
	else if (strcmp(command, "cls") == 0) {
		terminalClear();
	}
	else {
		printf("Unknown command: \"%s\"\r\n", command);
	}
}

void shellInit(const char *customPrompt) {
	if (customPrompt != NULL) {
		strcpy(prompt, customPrompt);
	}
	else {
		strcpy(prompt, "> ");
	}
}
