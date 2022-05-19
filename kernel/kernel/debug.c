#include <stdio.h>
#include <kernel/sourceFileInfo.h>
__attribute__ ((unused)) static sourceFileInfo fileInfo = {
	.fileName = "kernel/kernel/assert.c",
	.lastEditor = "Techflash",
	.lastEditDate = "May 14th, 2022",
	.lastEditReason = "Center \"End File Info Dump\" text in the line, as well as make the lines longer",
	.versionMajor = 0,
	.versionMinor = 0,
	.versionPatch = 2
};
void kernelDebug(const char *message, sourceFileInfo file, int line) {
	(void)fileInfo;
	printf("DEBUG: Message: \"%s\", Line: %d\r\n", message, line);
	printf("\
============================= BEGIN FILE INFO DUMP =============================\
FileName: %s\r\n\
LastEditor: %s\r\n\
LastEditTime: %s\r\n\
LastEditReason: %s\r\n\
Version: %d.%d.%d\r\n\
============================== END FILE INFO DUMP ==============================\r\n",
file.fileName,
file.lastEditor,
file.lastEditDate,
file.lastEditReason,
file.versionMajor, file.versionMinor, file.versionPatch
);
}
