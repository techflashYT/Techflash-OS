#ifndef SOURCEFILEINFO_H
#define SOURCEFILEINFO_H
#include <stdint.h>
typedef struct {
	const char *fileName;
	const char *lastEditor;
	const char *lastEditDate;
	const char *lastEditReason;
	uint8_t versionMajor;
	uint8_t versionMinor;
	uint8_t versionPatch;
} __attribute__ ((packed)) sourceFileInfo;
#endif
