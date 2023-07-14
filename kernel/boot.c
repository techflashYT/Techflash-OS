#include <kernel/custom.h>
#include <kernel/panic.h>
#include <kernel/graphics.h>
#include <kernel/environment.h>
#include <kernel/hardware/serial.h>
#include <kernel/hardware/PIT.h>
#include <stdlib.h>
#include <stdio.h>
#include <kernel/boot.h>
uint_fast8_t BP_MaxTasks          = 10;
double       BP_currentTasks      = 0.0f;
double       BP_Percent           = 0;
uint_fast8_t BP_x, BP_Y, BP_Width = 0;

void BP_Init(const uint_fast8_t x, const uint_fast8_t y, const uint_fast8_t width) {
	// FIXME: weird hack putting this here
	/* prevent losing 64 bytes of stack for kernel runtime */ {
		// int maj = CONFIG_KERN_VERSION_MAJOR;
		// int min = CONFIG_KERN_VERSION_MINOR;
		// int pat = CONFIG_KERN_VERSION_PATCH;
		char str[64];
		sprintf(str, "Techflash OS \e[1m\e[36mv%d.%d.%d\e[0m Loading...\r\n", 1, 0, 0);
		puts(str);
		serial.writeString(SERIAL_PORT_COM1, str);
	}
	// Set global vars so that we know these properties in other functions
	BP_Percent = 0;
	BP_x       = x;
	BP_Y       = y;
	BP_Width   = width;
	
	// Create the progress bar.
	putcAt('[', x, y, colors.vga.lgray);
	putcAt(']', x + width, y, colors.vga.lgray);

	for (uint8_t i = 0; i < (width - 1); i++) {
		putcAt('#', x + 1 + i, y, 0x454550);
	}
}

void BP_FadeOut() {
	DUMPREGS;
	double percent = (double)( (double)( BP_currentTasks / BP_MaxTasks ) * 100 );
	if (percent < 99.5f) {
		panic("BP_FadeOut(): percent less than 100!  Did you forget to add more tasks before updating maxTasks?", regs);
	}
	if (percent > 100.5f) {
		panic("BP_FadeOut(): percent greater than 100!  Did you forget to update maxTasks before adding more?", regs);
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
		putcAt('[', BP_x, BP_Y, bracketColor);
		putcAt(']', BP_x + BP_Width, BP_Y, bracketColor);
		for (uint8_t i = 0; i < (BP_Width - 1); i++) {
			putcAt('#', BP_x + 1 + i, BP_Y, hashColor);
		}
		if (hashColor == 0 && bracketColor == 0) {
			break;
		}
	}
}
static uint_fast8_t BP_Percent2coords(const uint_fast8_t percent) {
	// Calculate the x coords for the progress bar.
	return (BP_Width - 1) * percent / 100;
}

void BP_Update() {
	DUMPREGS;
	BP_currentTasks += 1.0f;
	uint_fast8_t percent = (uint_fast8_t)( (float)( BP_currentTasks / BP_MaxTasks ) * 100 );
	// Verify that the percent is valid.
	if (percent == 0 || percent > 100) {
		panic("BP_update(): Percent not within range 1-100", regs);
	}
	// Update the progress bar.
	uint_fast8_t i = 0;
	for (; i < BP_Percent2coords(percent); i++) {
		putcAt('#', BP_x + 1 + i, BP_Y, colors.vga.white);
	}
	if (percent != 100) {
		putcAt('#', BP_x + 1 + i, BP_Y, colors.vga.lgray - 0x101010);
	}
}
