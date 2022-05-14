#include <stdio.h>
#include <stdbool.h>
#if defined(__is_libk)
#include <kernel/tty.h>
#endif
#include <kernel/sourceFileInfo.h>
static sourceFileInfo fileInfo = {
	.fileName = "libc/stdio/putchar.c",
	.lastEditor = "Techflash",
	.lastEditDate = "May 13th, 2022",
	.lastEditReason = "Add soureceFileInfo to putchar",
	.versionMajor = 0,
	.versionMinor = 0,
	.versionPatch = 1
};

// TODO: Serial output?
int putchar(int ic) {
	(void)fileInfo;
#if defined(__is_libk)
	char c = (char)ic;
	terminalWrite(&c, sizeof(c));
#else
	// TODO: Implement stdio and the write system call.
#endif
	return ic;
}
