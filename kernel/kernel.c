/*
	Original version Copyright 2017 - 2021 bzt (bztsrc@gitlab)
	as part of the 'bootboot' repository on GitLab.
	Original file available at https://gitlab.com/bztsrc/bootboot/-/blob/master/mykernel/c/kernel.c
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


#include <external/bootboot.h>
#include <kernel/environment.h>
#include <kernel/tty.h>
#include <kernel/hardware/serial.h>
#include <kernel/hardware/PIT.h>
#include <kernel/boot.h>
#include <kernel/graphics.h>
#include <kernel/hardware/CPU/GDT.h>
#include <kernel/hardware/CPU/IDT.h>
#include <kernel/hardware/CPU/SSE.h>
#include <kernel/hardware/CPU/IRQ.h>
#include <kernel/panic.h>
#include <kernel/hardware/IO.h>
#include <kernel/hardware/kbd.h>

#include <kernel/misc.h>
uint8_t SSEFeaturesBits = 0;
void __initThings();
void initExceptions();
void PICInit();
void keyboardInit();
// cppcheck-suppress unusedFunction
/******************************************
 * Entry point, called by BOOTBOOT Loader *
 ******************************************/
extern char keyboardBuffer[256];
void _start() {
	// Disable interrupts during setup
	asm volatile ("cli");
	/*** NOTE: this code runs on all cores in parallel ***/
	int s = bootboot.fb_scanline;
	// int w = bootboot.fb_width;
	// int h = bootboot.fb_height;


	if(s) {
		SSEInit();
		__initThings();
		keyboard.setLED(KEYBOARD_LED_NUMLOCK, true);
		// Say that the kernel is loading and to please wait.
		puts("Techflash OS v");
		kernTTY.color = vga.colors.cyan + 0x002020;
		printf("%d.%d.%d ", CONFIG_KERN_VERSION_MAJOR, CONFIG_KERN_VERSION_MINOR, CONFIG_KERN_VERSION_PATCH);
		kernTTY.color = 0xAAAAAA;
		puts("Loading...\r\n");
		env = handleEnv();
		// if (env.experimental.progressBarBoot) {
		boot.progressBar.create((kernTTY.width / 2) - (kernTTY.width / 3), (kernTTY.height / 2) + (kernTTY.height / 8), kernTTY.width / 2);

		// Initialize the 8259 Programmable Interrupt Controller
		PICInit();

		// Initialize the Global Descriptor Table
		GDTInit();

		boot.progressBar.update(10);

		// Initialize the Interrupt Descriptor Table
		IDTInit();

		// Init the keyboard driver
		keyboardInit();

		setKeyboardInterruptState(0, false);
		// Initialize some exception handlers
		initExceptions();

		boot.progressBar.update(20);
		boot.progressBar.update(30);
		for (uint8_t i = 30; i < 99; i++) {
			boot.progressBar.update(i);
			// sleep(2000);
		}
		// Initialize the PIT to 60hz
		initPIT(60);
		// Disable PIT interrupt via PIC because for some reason it's busted
		IRQSetMask(0, true);
		IRQSetMask(8, true);
		// Enable keyboard interrupt via PIC
		IRQSetMask(1, true);
		IRQSetMask(9, true);

		setKeyboardInterruptState(0, true);
		// Now the interrupts are ready, enable them
		printf("INTERRUPTS ARE BEING ENABLED!\r\n");
		asm volatile ("sti");
	}
	while (true) {
		kernTTY.cursorX = 0;
		kernTTY.cursorY = 8;
		// Main kernel loop
		char userInput = keyboardBufferPop();
		if ((uint8_t)userInput == 0xFF) {
			// Special key
			printf("%s\r\n", kbdGetLastSpecialKey());
		}
		if (userInput != (char)'\0') {
			// putchar(userInput);
			printf("userInput: %c  \r", userInput);
		}
	}
	asm volatile (
		"cli\n"
		"hlt\n"
	);
}