#include <stdint.h>
extern void COM_Write(uint8_t serPort, uint8_t byte);
extern void COM_LogWrapper(const char ch, const uint16_t x, const uint16_t y, const uint32_t fgColor, const uint32_t bgColor);
extern void COM_WriteStr(uint8_t serPort, const char *str);