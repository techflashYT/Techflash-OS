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
// cppcheck-suppress unusedFunction
/******************************************
 * Entry point, called by BOOTBOOT Loader *
 ******************************************/
void _start() {
	/*** NOTE: this code runs on all cores in parallel ***/
	int s = bootboot.fb_scanline;
	// int w = bootboot.fb_width;
	// int h = bootboot.fb_height;
	__asm__("call __init"); // Not quite sure why, but I need to call the function to init the environment asm style in order for the compiler not to insert garbage in front of_start();
	// serial.init();
	// serial.write(SERIAL_PORT_COM1, 'a');
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
