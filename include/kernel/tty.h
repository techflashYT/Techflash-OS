#pragma once
#include <stdbool.h>
#include <stdint.h>
extern bool          TTY_Ready;
extern volatile bool TTY_Bold;
extern bool          TTY_BlinkingCursor;
extern uint_fast16_t TTY_CursorX;
extern uint_fast16_t TTY_CursorY;
extern uint_fast16_t TTY_CursorAfterPromptX;
extern uint32_t      TTY_Color;
extern uint32_t      TTY_TextBackground;
extern uint_fast32_t TTY_Width;
extern uint_fast32_t TTY_Height;
extern uint_fast32_t TTY_Index;

extern char          *TTY_PromptStr;
extern void           TTY_SetBackground(const uint32_t color);
extern void           TTY_PrintPrompt  ();
extern void           TTY_Blink        ();
extern void           TTY_Scroll       (const char *numLines);
extern void           TTY_Init         ();

extern bool           FB_HandleEsc(char nextChar);
extern void           FB_DrawChar(const char ch, const uint_fast16_t x, const uint_fast16_t y);