#include <stdio.h>
#include <stdbool.h>
#if defined(__is_libk)
#include <kernel/tty.h>
#endif
#include <kernel/sourceFileInfo.h>
static sourceFileInfo fileInfo = {
	.fileName = "libc/stdio/putchar.c",
	.lastEditor = "Techflash",
	.lastEditDate = "May 14th, 2022",
	.lastEditReason = "Add check for null",
	.versionMajor = 0,
	.versionMinor = 0,
	.versionPatch = 2
};

// TODO: Serial output?
int putchar(int ic) {
	(void)fileInfo;
#if defined(__is_libk)
	char c = (char)ic;
	if (c == '\0') {
		return 0;
	}
	terminalWrite(&c, sizeof(c));
#else
	// TODO: Implement stdio and the write system call.
#endif
	return ic;
}
