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

/******************************************
 * Entry point, called by BOOTBOOT Loader *
 ******************************************/
void _start() {
	/*** NOTE: this code runs on all cores in parallel ***/
	int s = bootboot.fb_scanline;
	// int w = bootboot.fb_width;
	// int h = bootboot.fb_height;

	if(s) {
		// Say that the kernel is loading and to please wait.
		puts("abcd");
	}
	// hang for now
	while(1);
}