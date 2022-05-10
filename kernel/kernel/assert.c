#include <stdio.h>
#include <kernel/sourceFileInfo.h>
void assert(const char *message, sourceFileInfo file, int line) {
	printf("ASSERT: Message: \"%s\", Line: %d\r\n", message, line);
	printf(" \
==== Begin File Info Dump ====\r\n \
FileName: %s\r\n \
LastEditor: %s\r\n \
LastEditTime: %s\r\n \
LastEditReason: %s\r\n \
Version: %d.%d.%d\r\n \
==== End File Info Dump ====\r\n",
file.fileName,
file.lastEditor,
file.lastEditDate,
file.lastEditReason,
file.versionMajor, file.versionMinor, file.versionPatch
);
}
