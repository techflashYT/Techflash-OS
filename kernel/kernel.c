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
#include <kernel/hardware/serial.h>
#include <kernel/hardware/PIT.h>
#include <kernel/boot.h>
#include <kernel/graphics.h>
#include <kernel/hardware/CPU/GDT.h>
#include <kernel/hardware/CPU/IDT.h>
#include <kernel/hardware/CPU/SSE.h>
#include <kernel/panic.h>
#include <kernel/hardware/IO.h>

#include <kernel/misc.h>
void __initThings();
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
		SSEInit();
		__initThings();
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
		boot.progressBar.update(10);
		IDTInit();
		boot.progressBar.update(20);
		boot.progressBar.update(30);
		for (uint8_t i = 30; i < 99; i++) {
			boot.progressBar.update(i);
			// sleep(2000);
		}
		// Initialize the PIT to 60hz
		initPIT(60);
	}
	while (true);
	asm volatile (
		"cli\n"
		"hlt\n"
	);
}