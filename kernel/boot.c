#include <kernel/custom.h>
#include <kernel/misc.h>
#include <kernel/panic.h>
#include <kernel/graphics.h>
#include <kernel/environment.h>
#include <kernel/hardware/serial.h>
#include <kernel/hardware/PIT.h>
#include <stdlib.h>
#include <kernel/boot.h>
void bootProgressBarCreate(const uint8_t x, const uint8_t y, const uint8_t width);
void bootProgressBarFadeOut();
void bootProgressBarUpdate(const uint8_t percent);
void bootInit() {
	boot.percent = 0;
	boot.progressBar.create  = bootProgressBarCreate;
	boot.progressBar.update  = bootProgressBarUpdate;
	boot.progressBar.fadeOut = bootProgressBarFadeOut;
}
uint8_t bootProgressBarX = 0;
uint8_t bootProgressBarY = 0;
uint8_t bootProgressBarWidth = 0;
void bootProgressBarCreate(const uint8_t x, const uint8_t y, const uint8_t width) {
	// Set global vars so that we know these properties in other functions
	bootProgressBarX = x;
	bootProgressBarY = y;
	bootProgressBarWidth = width;
	
	// Create the progress bar.
	putcAt('[', x, y, vga.colors.lgray);
	putcAt(']', x + width, y, vga.colors.lgray);
	for (uint8_t i = 0; i < (width - 1); i++) {
		putcAt('#', x + 1 + i, y, 0x454550);
	}
}
void bootProgressBarFadeOut() {
	uint32_t bracketColor = vga.colors.lgray;
	uint32_t hashColor    = vga.colors.white;
	while (true) {
		sleep(12);
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
		serial.writeString(SERIAL_PORT_COM1, "\r\n\r\n");
		free(utoaBuf);
		#endif
		putcAt('[', bootProgressBarX, bootProgressBarY, bracketColor);
		putcAt(']', bootProgressBarX + bootProgressBarWidth, bootProgressBarY, bracketColor);
		for (uint8_t i = 0; i < (bootProgressBarWidth - 1); i++) {
			putcAt('#', bootProgressBarX + 1 + i, bootProgressBarY, hashColor);
		}
		if (hashColor == 0 && bracketColor == 0) {
			break;
		}
	}
}
static uint8_t percentToCoords(const uint8_t percent) {
	// Calculate the x coords for the progress bar.
	return (bootProgressBarWidth - 1) * percent / 100;
}
void bootProgressBarUpdate(const uint8_t percent) {
	// Verify that the percent is valid.
	if (percent == 0 || percent > 100) {
		DUMPREGS
		panic("bootProgressBarUpdate(): Percent not within range 1-100", regs);
	}
	// Update the progress bar.
	uint8_t i = 0;
	for (; i < percentToCoords(percent); i++) {
		putcAt('#', bootProgressBarX + 1 + i, bootProgressBarY, vga.colors.white);
	}
	if (percent != 100) {
		putcAt('#', bootProgressBarX + 1 + i, bootProgressBarY, vga.colors.lgray - 0x101010);
	}
}