/*
	Original version Copyright 2017 - 2021 bzt (bztsrc@gitlab)
	as part of the 'bootboot' repository on GitLab.
	Original file availible at https://gitlab.com/bztsrc/bootboot/-/blob/master/mykernel/c/kernel.c
*/
#include <stdio.h>
#include <stdint.h>
#include <external/bootboot.h>
#include <kernel/environment.h>
#include <kernel/tty.h>
#include <kernel/hardware/kbd.h>
#include <kernel/hardware/serial.h>
extern void __kernTTY_init();
_kernTTY_t kernTTY;

extern bool __keyboardGetStatusOfLED(const uint8_t led);
extern void __keyboardSetLED(const uint8_t led, const bool value);
__keyboard_t keyboard;

extern int __serialInit();
extern char __serialReadNext(const uint16_t port);
extern void __serialWrite(const uint16_t port, const char value);
extern void __serialWriteString(const uint16_t port, const char* value);
__serial_t serial;



// cppcheck-suppress unusedFunction
/******************************************
 * Entry point, called by BOOTBOOT Loader *
 ******************************************/
void _start() {
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
	// Start setting up the serial struct.
	serial.init = __serialInit;
	serial.readNext = __serialReadNext;
	serial.write = __serialWrite;
	serial.writeString = __serialWriteString;

	serial.init();
	serial.writeString(SERIAL_PORT_COM1, "This is a serial test string!\r\nIf this goes through correctly, it worked!\r\n");

	if(s) {
		// Say that the kernel is loading and to please wait.
		puts("abcdefghijklnmop\r\n");
		// keyboard.setLED(KEYBOARD_LED_NUMLOCK, true);
		puts("abcdefghijklnmop\r\n");
		puts("abcdefghijklnmop\r\n");
	}
	// hang for now
	while(1);
}
