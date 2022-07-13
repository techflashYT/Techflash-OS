/*
	Original version Copyright 2017 - 2021 bzt (bztsrc@gitlab)
	as part of the 'bootboot' repository on GitLab.
	Original file availible at https://gitlab.com/bztsrc/bootboot/-/blob/master/mykernel/c/kernel.c
*/
/* function to display a string, see below */
#include <stdio.h>
#include <stdint.h>
#include <external/bootboot.h>
#include <kernel/environment.h>
#include <kernel/tty.h>
#include <kernel/hardware/kbd.h>
extern void __kernTTY_init();
_kernTTY_t kernTTY;

extern bool __keyboardGetStatusOfLED(uint8_t led);
extern void __keyboardSetLED(uint8_t led, bool value);
__keyboard_t keyboard;

// cppcheck-suppress unusedFunction
/******************************************
 * Entry point, called by BOOTBOOT Loader *
 ******************************************/
void _start() {
	__asm__ ("cli\n""hlt");
	/*** NOTE: this code runs on all cores in parallel ***/
	int s = bootboot.fb_scanline;
	// int w = bootboot.fb_width;
	// int h = bootboot.fb_height;

	// Start initializing a TTY.
	kernTTY.init = __kernTTY_init;
	kernTTY.init();

	// Start setting up the keyboard struct.
	keyboard.getStatusLED = __keyboardGetStatusOfLED;
	keyboard.setLED = __keyboardSetLED;

	if(s) {
		// Say that the kernel is loading and to please wait.
		puts("abcdefghijklnmop\r\n");
		keyboard.setLED(KEYBOARD_LED_NUMLOCK, true);
		puts("abcdefghijklnmop\r\n");
		puts("abcdefghijklnmop\r\n");
	}
	// hang for now
	while(1);
}
