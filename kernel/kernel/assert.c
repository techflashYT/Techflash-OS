#include <stdio.h>
#include <kernel/sourceFileInfo.h>
static sourceFileInfo fileInfo = {
	.fileName = "kernel/kernel/assert.c",
	.lastEditor = "Techflash",
	.lastEditDate = "May 13th, 2022",
	.lastEditReason = "Add soureceFileInfo to assert",
	.versionMajor = 0,
	.versionMinor = 0,
	.versionPatch = 1
};
void assert(const char *message, sourceFileInfo file, int line) {
	(void)fileInfo;
	printf("ASSERT: Message: \"%s\", Line: %d\r\n", message, line);
	printf("\
==== Begin File Info Dump ====\r\n\
FileName: %s\r\n\
LastEditor: %s\r\n\
LastEditTime: %s\r\n\
LastEditReason: %s\r\n\
Version: %d.%d.%d\r\n\
==== End File Info Dump ====\r\n",
file.fileName,
file.lastEditor,
file.lastEditDate,
file.lastEditReason,
file.versionMajor, file.versionMinor, file.versionPatch
);
}
