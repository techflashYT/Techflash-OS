#include <stdint.h>
typedef void (*TTY_WriteFunc_t)(const char ch, const uint16_t x, const uint16_t y, const uint32_t fgColor, const uint32_t bgColor);

extern uint16_t TTY_CursorX;
extern uint16_t TTY_CursorY;
extern uint16_t TTY_Width;
extern uint16_t TTY_Height;

extern uint32_t TTY_Color;
extern uint32_t TTY_BgColor;
extern TTY_WriteFunc_t TTY_WriteFunc;

extern void TTY_SetWriteFunc(TTY_WriteFunc_t func);
extern void TTY_Write(const char ch, const uint16_t x, const uint16_t y, const uint32_t fgColor, const uint32_t bgColor);
