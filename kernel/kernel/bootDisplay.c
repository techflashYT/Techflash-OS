#include <kernel/arch/i386/vga.h>
#include <kernel/tty.h>
#include <kernel/panic.h>
#include <stdint.h>
#include <stdio.h>
#define BOOTDISPLAY_NUM_ACTIONS 4
extern uint8_t bootProgress;
uint8_t bootDisplayProgressBarLastLocation = 1;
void bootDisplayMakeBrackets(uint8_t y) {
	terminalRow = 71;
	terminalColumn = y;
	terminalColor = VGA_COLOR_LIGHT_GRAY;
	terminalPutchar('[');
	terminalColor = VGA_COLOR_YELLOW;
	puts("WAIT");
	terminalColor = VGA_COLOR_LIGHT_GRAY;
	terminalPutchar(']');
}


void bootDisplayOK(uint8_t y) {
	terminalRow = 72;
	terminalColumn = y;
	terminalColor = VGA_COLOR_BLACK;
	terminalPutchar('\0');
	terminalColor = VGA_COLOR_GREEN;
	puts("OK");
	terminalPutchar('\0');
}

void bootDisplayFAIL(uint8_t y) {
	terminalRow = 69;
	terminalColumn = y;
	terminalColor = VGA_COLOR_LIGHT_RED;
	puts("**");
	terminalColor = VGA_COLOR_YELLOW;
	terminalPutchar('[');
	terminalColor = VGA_COLOR_LIGHT_RED;
	puts("FAIL");
	terminalColor = VGA_COLOR_YELLOW;
	terminalPutchar(']');
	terminalColor = VGA_COLOR_LIGHT_RED;
	puts("**");
}
void bootDisplayMakeProgressBar(void) {
	terminalPutEntryAt('[', VGA_COLOR_LIGHT_GRAY, 0, 20);
	terminalPutEntryAt(']', VGA_COLOR_LIGHT_GRAY, 79, 20);
	for (uint8_t i = 1; i <= 78; i++) {
		terminalPutEntryAt('#', VGA_COLOR_DARK_GRAY, i, 20);
	}
}
uint8_t bootDisplayProgressToLocation(void) {
	uint8_t e = (uint8_t)((bootProgress * 78) / BOOTDISPLAY_NUM_ACTIONS);
	if (e >= 80) {
		panic("Progress bar went too far!");
	}
	return e;
}
void bootDisplayProgressBarFail(void) {
		for (uint8_t i = 0; i <= bootDisplayProgressToLocation(); i++) {
		terminalPutEntryAt('#', VGA_COLOR_RED, i, 20);
	}
}
void bootDisplayProgressBarUpdate(void) {
	uint8_t i;

	for (i = bootDisplayProgressBarLastLocation; i <= bootDisplayProgressToLocation(); i++) {
		terminalPutEntryAt('#', VGA_COLOR_WHITE, i, 20);
	}
	bootDisplayProgressBarLastLocation = i;
}
