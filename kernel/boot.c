#include <kernel/custom.h>
#include <kernel/misc.h>
#include <kernel/panic.h>
#include <kernel/graphics.h>
#include <kernel/environment.h>
#include <kernel/hardware/serial.h>
#include <kernel/hardware/PIT.h>
#include <stdlib.h>
#include <kernel/boot.h>
uint8_t BP_maxTasks     = 10;
double  BP_currentTasks = 0.0f;
double  BP_percent      = 0;
uint8_t BP_x, BP_y, BP_width = 0;

void BP_init(const uint8_t x, const uint8_t y, const uint8_t width) {
	// Set global vars so that we know these properties in other functions
	BP_percent = 0;
	BP_x     = x;
	BP_y     = y;
	BP_width = width;
	
	// Create the progress bar.
	putcAt('[', x, y, colors.vga.lgray);
	putcAt(']', x + width, y, colors.vga.lgray);

	for (uint8_t i = 0; i < (width - 1); i++) {
		putcAt('#', x + 1 + i, y, 0x454550);
	}
}

void BP_fadeOut() {
	DUMPREGS;
	double percent = (double)( (double)( BP_currentTasks / BP_maxTasks ) * 100 );
	if (percent < 99.5f) {
		panic("bootProgressBarFadeOut(): percent less than 100!  Did you forget to add more tasks before updating maxTasks?", regs);
	}
	if (percent > 100.5f) {
		panic("bootProgressBarFadeOut(): percent greater than 100!  Did you forget to update maxTasks before adding more?", regs);
	}
	
	uint32_t bracketColor = colors.vga.lgray;
	uint32_t hashColor    = colors.vga.white;

	while (true) {
		sleep(12);
		// TODO: Improve this
		if (hashColor != 0x00000000) {
			if (hashColor == 0x30303) {
				hashColor = 0x00000000;
			}
			else {
				hashColor -= 0x00040404;
			}
		}
		if (bracketColor != 0x00000000) {
			if (bracketColor == 0x20202) {
				bracketColor = 0x00000000;
			}
			else {
				bracketColor -= 0x00030303;
			}
		}
		// #define BOOTPROGRESSBAR_DEBUG
		#ifdef BOOTPROGRESSBAR_DEBUG
		char *utoaBuf = malloc(32);
		serial.writeString(SERIAL_PORT_COM1, "hcol: ");
		utoa(hashColor, utoaBuf, 16);
		serial.writeString(SERIAL_PORT_COM1, utoaBuf);
		serial.writeString(SERIAL_PORT_COM1, "\r\n");
		serial.writeString(SERIAL_PORT_COM1, "bcol: ");
		utoa(bracketColor, utoaBuf, 16);
		serial.writeString(SERIAL_PORT_COM1, utoaBuf);
		free(utoaBuf);
		serial.writeString(SERIAL_PORT_COM1, "\r\n\r\n");
		#endif
		putcAt('[', BP_x, BP_y, bracketColor);
		putcAt(']', BP_x + BP_width, BP_y, bracketColor);
		for (uint8_t i = 0; i < (BP_width - 1); i++) {
			putcAt('#', BP_x + 1 + i, BP_y, hashColor);
		}
		if (hashColor == 0 && bracketColor == 0) {
			break;
		}
	}
}
static uint8_t BP_percent2coords(const uint8_t percent) {
	// Calculate the x coords for the progress bar.
	return (BP_width - 1) * percent / 100;
}

void BP_update() {
	DUMPREGS;
	BP_currentTasks += 1.0f;
	uint8_t percent = (uint8_t)( (float)( BP_currentTasks / BP_maxTasks ) * 100 );
	// Verify that the percent is valid.
	if (percent == 0 || percent > 100) {
		panic("BP_update(): Percent not within range 1-100", regs);
	}
	// Update the progress bar.
	uint8_t i = 0;
	for (; i < percentToCoords(percent); i++) {
		putcAt('#', BP_x + 1 + i, BP_y, colors.vga.white);
	}
	if (percent != 100) {
		putcAt('#', BP_x + 1 + i, BP_y, colors.vga.lgray - 0x101010);
	}
}