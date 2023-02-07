#pragma once
#include <stdbool.h>
#include <stdint.h>
extern bool     TTY_Ready;
extern bool     TTY_BlinkingCursor;
extern uint16_t TTY_CursorX;	
extern uint16_t TTY_CursorY;
extern uint16_t TTY_CursorAfterPromptX;
extern uint32_t TTY_Color;
extern uint32_t TTY_TextBackground;
extern uint32_t TTY_Width;
extern uint32_t TTY_Height;
extern uint32_t TTY_Index;

extern char    *TTY_PromptStr;
extern void     TTY_SetBackground(const uint32_t color);
extern void     TTY_PrintPrompt  ();
extern void     TTY_Blink        ();
extern void     TTY_Scroll       (const char *numLines);
extern void     TTY_Init         ();
