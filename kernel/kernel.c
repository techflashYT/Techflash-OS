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
#include <kernel/custom.h>
#include <kernel/hardware/kbd.h>
#include <kernel/hardware/serial.h>
#include <kernel/hardware/PIT.h>
#include <kernel/boot.h>
#include <kernel/graphics.h>
#include <kernel/hardware/GDT.h>
#include <kernel/hardware/IDT.h>

#include <kernel/misc.h>
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

extern void __bootProgressBarUpdate(const uint8_t percent);
extern void __bootProgressBarCreate(const uint8_t x, const uint8_t y, const uint8_t width);
__boot_t boot;

extern uint32_t __VGAColorBlack;
extern uint32_t __VGAColorBlue;
extern uint32_t __VGAColorGreen;
extern uint32_t __VGAColorCyan;
extern uint32_t __VGAColorRed;
extern uint32_t __VGAColorPurple;
extern uint32_t __VGAColorBrown;
extern uint32_t __VGAColorLGray;
extern uint32_t __VGAColorDGray;
extern uint32_t __VGAColorLBlue;
extern uint32_t __VGAColorLGreen;
extern uint32_t __VGAColorLCyan;
extern uint32_t __VGAColorLRed;
extern uint32_t __VGAColorPink;
extern uint32_t __VGAColorYellow;
extern uint32_t __VGAColorWhite;
__vga vga;

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
		boot.progressBar.update = __bootProgressBarUpdate;
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

		vga.colors.black = __VGAColorBlack;
		vga.colors.blue = __VGAColorBlue;
		vga.colors.green = __VGAColorGreen;
		vga.colors.cyan = __VGAColorCyan;
		vga.colors.red = __VGAColorRed;
		vga.colors.purple = __VGAColorPurple;
		vga.colors.brown = __VGAColorBrown;
		vga.colors.lgray = __VGAColorLGray;
		vga.colors.dgray = __VGAColorDGray;
		vga.colors.lblue = __VGAColorLBlue;
		vga.colors.lgreen = __VGAColorLGreen;
		vga.colors.lcyan = __VGAColorLCyan;
		vga.colors.lred = __VGAColorLRed;
		vga.colors.pink = __VGAColorPink;
		vga.colors.yellow = __VGAColorYellow;
		vga.colors.white = __VGAColorWhite;

		// Say that the kernel is loading and to please wait.
		puts("Techflash OS v");
		kernTTY.color = vga.colors.cyan + 0x002020;
		printf("%d.%d.%d ", CONFIG_KERN_VERSION_MAJOR, CONFIG_KERN_VERSION_MINOR, CONFIG_KERN_VERSION_PATCH);
		kernTTY.color = 0xAAAAAA;
		puts("Loading...\r\n");

		env = handleEnv();

		// if (env.experimental.progressBarBoot) {
		boot.progressBar.create((kernTTY.width / 2) - (kernTTY.width / 3), (kernTTY.height / 2) + (kernTTY.height / 8), kernTTY.width / 2);
		// Initialize the GDT
		GDTInit();
		BREAK
		boot.progressBar.update(10);
		IDTInit();
		boot.progressBar.update(20);
		// Initialize the PIT
		initPIT();
		boot.progressBar.update(30);
		for (uint8_t i = 30; i < 99; i++) {
			boot.progressBar.update(i);
			sleep(100);
		}
		

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