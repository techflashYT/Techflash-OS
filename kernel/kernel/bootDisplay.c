#include <kernel/arch/i386/vga.h>
#include <kernel/tty.h>
#include <kernel/panic.h>
#include <stdint.h>
#define BOOTDISPLAY_NUM_ACTIONS 4
extern uint8_t bootProgress;
uint8_t bootDisplayProgressBarLastLocation = 1;
void bootDisplayMakeBrackets(uint8_t y) {
	terminalPutEntryAt('[', VGA_COLOR_LIGHT_GRAY, 71, y);
	terminalPutEntryAt('W', VGA_COLOR_YELLOW, 72, y);
	terminalPutEntryAt('A', VGA_COLOR_YELLOW, 73, y);
	terminalPutEntryAt('I', VGA_COLOR_YELLOW, 74, y);
	terminalPutEntryAt('T', VGA_COLOR_YELLOW, 73, y);
	terminalPutEntryAt(']', VGA_COLOR_LIGHT_GRAY, 76, y);
}


void bootDisplayOK(uint8_t y) {
	terminalPutEntryAt('\0', VGA_COLOR_LIGHT_GRAY, 72, y);
	terminalPutEntryAt('O', VGA_COLOR_GREEN, 73, y);
	terminalPutEntryAt('K', VGA_COLOR_GREEN, 74, y);
	terminalPutEntryAt('\0', VGA_COLOR_LIGHT_GRAY, 75, y);
}

void bootDisplayFAIL(uint8_t y) {
	terminalPutEntryAt('*', VGA_COLOR_LIGHT_RED, 69, y);
	terminalPutEntryAt('*', VGA_COLOR_LIGHT_RED, 70, y);
	terminalPutEntryAt('[', VGA_COLOR_YELLOW, 71, y);
	terminalPutEntryAt('F', VGA_COLOR_RED, 72, y);
	terminalPutEntryAt('A', VGA_COLOR_RED, 73, y);
	terminalPutEntryAt('I', VGA_COLOR_RED, 74, y);
	terminalPutEntryAt('L', VGA_COLOR_RED, 75, y);
	terminalPutEntryAt(']', VGA_COLOR_YELLOW, 76, y);
	terminalPutEntryAt('*', VGA_COLOR_LIGHT_RED, 77, y);
	terminalPutEntryAt('*', VGA_COLOR_LIGHT_RED, 78, y);
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
