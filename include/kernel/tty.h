#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <external/flanterm/flanterm.h>
#include <external/flanterm/backends/fb.h>
extern bool          TTY_Ready;
extern volatile bool TTY_Bold;
extern bool          TTY_BlinkingCursor;
extern uint_fast16_t TTY_CursorX;
extern uint_fast16_t TTY_CursorY;
extern uint_fast32_t TTY_Color;
extern uint_fast32_t TTY_TextBackground;
extern uint_fast16_t TTY_Width;
extern uint_fast16_t TTY_Height;
extern uint_fast32_t TTY_Index;
extern struct flanterm_context *TTY_Ctx;

extern void           TTY_SetBackground(const uint32_t color);
extern void           TTY_Blink        ();
extern void           TTY_Scroll       (const uint_fast16_t numLines);
extern void           TTY_Init         ();

typedef struct {
	char ch;
	uint16_t x, y;
	uint32_t color;
	uint32_t background;
} bufferEntry_t;

extern bool           FB_HandleEsc(char nextChar);
extern void           FB_WriteChar(const char ch, const uint_fast16_t x, const uint_fast16_t y);
extern void           FB_ReInit   (bufferEntry_t *newBuffer, int newBufferSize);
extern void           FB_Update   ();

extern void           FB_DrawLine     (uint_fast16_t y);
extern void           FB_DeleteLine   (uint_fast16_t y);
extern void           FB_SetBufferItem(uint_fast16_t x, uint_fast16_t y, bufferEntry_t entry);
extern bufferEntry_t  FB_GetBufferItem(uint_fast16_t x, uint_fast16_t y);