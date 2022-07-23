/*
	Original version Copyright 2017 - 2021 bzt (bztsrc@gitlab)
	as part of the 'bootboot' repository on GitLab.
	Original file availible at https://gitlab.com/bztsrc/bootboot/-/blob/master/mykernel/c/kernel.c
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <external/bootboot.h>
#include <kernel/environment.h>
#include <kernel/tty.h>
#include <kernel/hardware/kbd.h>
#include <kernel/hardware/serial.h>
#include <kernel/boot.h>
extern void __kernTTY_init();
_kernTTY_t kernTTY;

extern bool __keyboardGetStatusOfLED(const uint8_t led);
extern void __keyboardSetLED(const uint8_t led, const bool value);
__keyboard_t keyboard;

extern char __serialReadNext(const uint16_t port);
extern void __serialWrite(const uint16_t port, const char value);
extern void __serialWriteString(const uint16_t port, const char* value);
__serial_t serial;

__environment_t env;


extern void __bootProgressBarCreate(const uint8_t x, const uint8_t y, const uint8_t width);
__boot_t boot;

// cppcheck-suppress unusedFunction
/******************************************
 * Entry point, called by BOOTBOOT Loader *
 ******************************************/
void _start() {
	/*** NOTE: this code runs on all cores in parallel ***/
	int s = bootboot.fb_scanline;
	// int w = bootboot.fb_width;
	// int h = bootboot.fb_height;


	if(s) {
		boot.percent = 0;
		boot.progressBar.create = __bootProgressBarCreate;
		// Start setting up the serial struct.
		serial.readNext = __serialReadNext;
		serial.write = __serialWrite;
		serial.writeString = __serialWriteString;

		// Start initializing a TTY.
		kernTTY.init = __kernTTY_init;
		kernTTY.init();

		// Start setting up the keyboard struct.
		keyboard.getStatusLED = __keyboardGetStatusOfLED;
		keyboard.setLED = __keyboardSetLED;
		keyboard.setLED(KEYBOARD_LED_NUMLOCK, true);

		// Say that the kernel is loading and to please wait.
		puts("Techflash OS v      Loading...\r\n");

		// cppcheck-suppress legacyUninitvar
		char *buffer = itoa(CONFIG_KERN_VERSION_MAJOR, buffer, 10);
		putcAt(buffer[0], 14, 0, 0xFFFFFF);
		putcAt('.', 15, 0, 0xFFFFFF);
		itoa(CONFIG_KERN_VERSION_MINOR, buffer, 10);
		putcAt(buffer[0], 16, 0, 0xFFFFFF);
		putcAt('.', 17, 0, 0xFFFFFF);
		itoa(CONFIG_KERN_VERSION_PATCH, buffer, 10);
		putcAt(buffer[0], 18, 0, 0xFFFFFF);

		env = handleEnv();

		// if (env.experimental.progressBarBoot) {
		boot.progressBar.create((kernTTY.width / 2) - (kernTTY.width / 3), (kernTTY.height / 2) + (kernTTY.height / 8), kernTTY.width / 2);
		// }
		// else {
			// boot.progressBar.create(20, 20, 50);
		// }
	}
	asm volatile (
		"cli\n"
		"hlt\n"
	);
}
