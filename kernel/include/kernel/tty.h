#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>

void terminalInit(void);
void terminalPutchar(char c);
void terminalWrite(const char* data, size_t size);
void terminalWritestring(const char* data);
void terminalMove(size_t x, size_t y);
size_t terminalGetX();
size_t terminalGetY();

#endif
