#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stdint.h>
#include <stddef.h>
void terminalInit(void);
void terminalScrollDown();
void terminalSetColor(uint8_t color);
void terminalPutEntryAt(char c, uint8_t color, size_t x, size_t y);
void terminalPutchar(char c);
void terminalWrite(const char* data, size_t size);
void terminalWriteString(const char* data);
#endif
