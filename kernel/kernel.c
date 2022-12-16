/*
	Original version Copyright 2017 - 2021 bzt (bztsrc@gitlab)
	as part of the 'bootboot' repository on GitLab.
	Original file available at https://gitlab.com/bztsrc/bootboot/-/blob/master/mykernel/c/kernel.c
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

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
#include <kernel/fs/tar.h>
#include <kernel/shell.h>
#include <kernel/memory.h>
#include <kernel/elf.h>
#include <kernel/arch.h>
#include <kernel/log.h>
#include <kernel/syscall.h>

#include <kernel/custom.h>

#include <kernel/misc.h>
uint8_t SSEFeaturesBits = 0;
void __initThings();
void initExceptions();
void PICInit();
void keyboardInit();
uint8_t maxTasks = 11;
float currentTasks = 0.0f;
extern bool timerReady;
// cppcheck-suppress unusedFunction
void kernelMain() {
	/*** NOTE: this code runs on all cores in parallel ***/
	// int s = bootboot.fb_scanline;
	// int w = bootboot.fb_width;
	// int h = bootboot.fb_height;
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
	currentTasks += 1.0f;
	boot.progressBar.update((uint8_t)( (float)( currentTasks / maxTasks ) * 100 ));

	// Initialize the Global Descriptor Table
	GDTInit();

	currentTasks += 1.0f;
	boot.progressBar.update((uint8_t)( (float)( currentTasks / maxTasks ) * 100 ));

	// Initialize the Interrupt Descriptor Table
	IDTInit();
	currentTasks += 1.0f;
	boot.progressBar.update((uint8_t)( (float)( currentTasks / maxTasks ) * 100 ));

	mallocInit((void*)0x0000000001000000);
	// Init the keyboard driver
	keyboardInit();
	currentTasks += 1.0f;
	boot.progressBar.update((uint8_t)( (float)( currentTasks / maxTasks ) * 100 ));	

	setKeyboardInterruptState(0, false);
	currentTasks += 1.0f;
	boot.progressBar.update((uint8_t)( (float)( currentTasks / maxTasks ) * 100 ));	
	// Initialize some exception handlers
	initExceptions();
	currentTasks += 1.0f;
	boot.progressBar.update((uint8_t)( (float)( currentTasks / maxTasks ) * 100 ));	

	serial.init();
	// Initialize the PIT to once every 1ms
	initPIT(1000);
	currentTasks += 1.0f;
	boot.progressBar.update((uint8_t)( (float)( currentTasks / maxTasks ) * 100 ));	

	currentTasks += 1.0f;
	boot.progressBar.update((uint8_t)( (float)( currentTasks / maxTasks ) * 100 ));

	setKeyboardInterruptState(0, true);
	currentTasks += 1.0f;
	boot.progressBar.update((uint8_t)( (float)( currentTasks / maxTasks ) * 100 ));
	// Now the interrupts are ready, enable them
	log("KERNEL", "INTERRUPTS ARE BEING ENABLED!!!");
	asm volatile ("sti");
	currentTasks += 1.0f;
	boot.progressBar.update((uint8_t)( (float)( currentTasks / maxTasks ) * 100 ));

	initSyscalls();
	parseTar((void *)bootboot.initrd_ptr);
	currentTasks += 1.0f;
	boot.progressBar.update((uint8_t)( (float)( currentTasks / maxTasks ) * 100 ));
	sleep(250);
	timerReady = true;
	boot.progressBar.fadeOut();
	/* FIXME: avoid reading the file because it's super busted for some reason
	uint8_t *outPtr = 0;
	size_t size = readFile((uint8_t *)bootboot.initrd_ptr, "test", &outPtr);
	*/
	extern uint8_t _binary_test_size;
	extern uint8_t _binary_test_start;
	extern uint8_t _binary_test_end;
	size_t size = (&_binary_test_end - &_binary_test_start);
	
	uint8_t *outPtr = &_binary_test_start;
	// asm("cli; hlt");
	printf("outptr: %p\r\n", outPtr);
	printf("size: %llu\r\n", size);
	printf("first 4 bytes: 0x%x, 0x", outPtr[0]);
	printf("%x, 0x", outPtr[1]);
	printf("%x, 0x", outPtr[2]);
	printf("%x\r\n", outPtr[3]);
	log("KERNEL", "Loading `test` binary!");
	elfStruct_t *address = elfLoader.load(outPtr, size);
	char *logBuffer = malloc(32);
	strcpy(logBuffer, "address of elf: 0x");
	utoa((uint64_t)address->startOfData, logBuffer + strlen(logBuffer), 16);
	log("KERNEL", logBuffer);
	free(logBuffer);

	int (*addrToCall)() = (void *)(address->startOfData + address->entryPointOffset);
	logBuffer = malloc(32);
	strcpy(logBuffer, "address of entryPoint: 0x");
	utoa((uint64_t)addrToCall, logBuffer + strlen(logBuffer), 16);
	log("KERNEL", logBuffer);
	free(logBuffer);
	log("KERNEL", "ELF Loaded, attempting to launch.");
	addrToCall();
	log("KERNEL", "HOLY CRAP THAT WORKED?!?!?!");
	kernTTY.printPrompt();
	kernTTY.blinkingCursor = true;
	kernTTY.cursorAfterPromptX = 0;
	char *command = malloc(512);
	uint16_t commandStrIndex = 0;
	while (true) {
		// Main kernel loop
		char userInput = keyboardGetLastKey();
		if ((uint8_t)userInput == 0xFF) {
			// Special key
			char *specialKey = kbdGetLastSpecialKey();
			if (specialKey[0] == '\r' && specialKey[1] == '\n') {
				// Enter
				puts("\r\n");
				command[commandStrIndex] = '\0';
				uint8_t val = handleCommands(command);
				if (val == 1) {
					printf("Unknown command: \'%s\'\r\n", command);
				}
				memset(command, 0, commandStrIndex); // zero out the string

				if (strcmp(kernTTY.promptStr, "") == 0) {
					DUMPREGS
					panic("DEBUG: Prompt was overwritten!  Malloc is trashed!", regs);
				}
				commandStrIndex = 0;
				kernTTY.printPrompt();
			}
			else if (specialKey[0] == '\b') {
				// Backspace
				if (kernTTY.cursorAfterPromptX != 0) {
					putcAt(' ', kernTTY.cursorX - 1, kernTTY.cursorY, kernTTY.color);
					kernTTY.cursorX--;
					kernTTY.cursorAfterPromptX--;
					commandStrIndex--;
				}
			}
		}
		if (userInput != (char)'\0' && (uint8_t)userInput != 0xFF) {
			putchar(userInput);
			command[commandStrIndex] = userInput;
			commandStrIndex++;
		}
	}
	asm volatile (
		"cli\n"
		"hlt\n"
	);
}