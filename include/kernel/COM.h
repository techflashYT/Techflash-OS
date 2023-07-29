#include <stdint.h>
extern void COM_Write(uint8_t serPort, uint8_t byte);
extern void COM_WriteStr(uint8_t serPort, const char *str);