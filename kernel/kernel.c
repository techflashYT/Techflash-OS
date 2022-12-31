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

#include <kernel/environment.h>
#include <kernel/tty.h>
#include <kernel/hardware/serial.h>
#include <kernel/hardware/parallel.h>
#include <kernel/hardware/PIT.h>
#include <kernel/boot.h>
#include <kernel/graphics.h>
#include <kernel/hardware/CPU/GDT.h>
#include <kernel/hardware/CPU/IDT.h>
#include <kernel/hardware/CPU/SSE.h>
#include <kernel/hardware/CPU/IRQ.h>
#include <kernel/panic.h>
#include <kernel/hardware/kbd.h>
#include <kernel/fs/tar.h>
#include <kernel/shell.h>
#include <kernel/memory.h>
#include <kernel/elf.h>
#include <kernel/syscall.h>

#include <kernel/custom.h>

#include <kernel/misc.h>
MODULE("KERNEL");
kernel_t kernel;

uint8_t SSEFeaturesBits = 0;
void initThings();
void initExceptions();
void PICInit();
void keyboardInit();
uint8_t maxTasks = 10;
extern bool timerReady;
// cppcheck-suppress unusedFunction
void kernelMain() {
	/*** NOTE: this code runs on all cores in parallel ***/
	// int s = bootboot.fb_scanline;
	// int w = bootboot.fb_width;
	// int h = bootboot.fb_height;
	SSEInit();
	// This is safe here because all it does it set a variable.
	mallocInit((void*)0x0000000001000000);
	initThings();
	// initialize serial logging.
	serial.init(115200);

	keyboard.setLED(KEYBOARD_LED_NUMLOCK, true);

	// Say that the kernel is loading and to please wait.
	puts("Techflash OS v");
	serial.writeString(SERIAL_PORT_COM1, "Techflash OS v");
	kernTTY.color = vga.colors.cyan + 0x002020;
	printf("%d.%d.%d", CONFIG_KERN_VERSION_MAJOR, CONFIG_KERN_VERSION_MINOR, CONFIG_KERN_VERSION_PATCH);
	kernTTY.color = vga.colors.lgray;

	serial.write(SERIAL_PORT_COM1, CONFIG_KERN_VERSION_MAJOR + '0');
	serial.write(SERIAL_PORT_COM1, '.');
	serial.write(SERIAL_PORT_COM1, CONFIG_KERN_VERSION_MINOR + '0');
	serial.write(SERIAL_PORT_COM1, '.');
	serial.write(SERIAL_PORT_COM1, CONFIG_KERN_VERSION_PATCH + '0');

	puts(" Loading...\r\n");
	serial.writeString(SERIAL_PORT_COM1, " Loading...\r\n");

	env = handleEnv();
	// if (env.experimental.progressBarBoot) {
	uint8_t bootX = ((kernTTY.width / 2) - (kernTTY.width / 3)); // idk it looks centered to me
	uint8_t bootY = ((kernTTY.height / 2) + (kernTTY.height / 4)); // don't forget, the Y goes up the farther down the screen you are.  this means 3/4 down the screen
	boot.progressBar.create(bootX, bootY, kernTTY.width / 2);

	// Initialize the 8259 Programmable Interrupt Controller
	PICInit();
	boot.progressBar.update();

	// Initialize the Global Descriptor Table
	GDTInit();
	boot.progressBar.update();

	// Initialize the Interrupt Descriptor Table
	IDTInit();
	boot.progressBar.update();

	// Init the keyboard driver
	keyboardInit();
	keyboard.setIntState(0, false);
	boot.progressBar.update();

	// Initialize some exception handlers
	initExceptions();
	boot.progressBar.update();

	// Initialize the PIT to once every 1ms
	initPIT(1000);
	boot.progressBar.update();
	
	keyboard.setIntState(0, true);
	
	// Now the interrupts are ready, enable them
	log(MODNAME, "INTERRUPTS ARE BEING ENABLED!!!", LOGLEVEL_WARN);
	asm volatile ("sti");
	timerReady = true;
	boot.progressBar.update();

	// we have the PIC, now lets unmask some interrupts
	// IRQSetMask(3, false);
	// IRQSetMask(4, false);
	// Initialize the parallel port (we need interrupts for this, since it has a timeout)
	parallel.init();
	boot.progressBar.update();

	initSyscalls();
	boot.progressBar.update();
	parseTar((void *)bootboot.initrd_ptr);
	boot.progressBar.update();
	
	sleep(250);
	
	boot.progressBar.fadeOut();
	/* FIXME: avoid reading the file because it's super busted for some reason
	uint8_t *outPtr = 0;
	size_t size = readFile((uint8_t *)bootboot.initrd_ptr, "test", &outPtr);
	*/
	// FIXME: Once the elf parser works, remove the if 0
	#if 0
	extern uint8_t _binary_test_size;
	extern uint8_t _binary_test_start;
	extern uint8_t _binary_test_end;
	size_t size = (&_binary_test_end - &_binary_test_start);
	
	uint8_t *outPtr = &_binary_test_start;
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Warray-bounds"
	// asm("cli; hlt");
	printf("outptr: %p\r\n", outPtr);
	printf("size: %llu\r\n", size);
	printf("first 4 bytes: 0x%x, 0x", outPtr[0]);
	printf("%x, 0x", outPtr[1]);
	printf("%x, 0x", outPtr[2]);
	printf("%x\r\n", outPtr[3]);
	#pragma GCC diagnostic pop
	log(MODNAME, "Loading `test` binary!", LOGLEVEL_DEBUG);
	uint8_t elfValid = elfLoader.isValid(outPtr, ARCH_X86_64);
	DUMPREGS
	if (elfValid == 1) {
		panic("Init binary is not a valid ELF!", regs);
	}
	else if (elfValid == 2 || elfValid == 3) {
		panic("Init binary is a valid ELF, but not for this CPU!", regs);
	}
	elfStruct_t *address = elfLoader.load(outPtr);
	if (address->err) {
		panic("Error loading init.  The serial log may have more info.", regs);
	}
	char *logBuffer = malloc(32);
	strcpy(logBuffer, "address of elf: 0x");
	utoa((uint64_t)address->startOfData, logBuffer + strlen(logBuffer), 16);
	log(MODNAME, logBuffer, LOGLEVEL_DEBUG);
	free(logBuffer);

	int (*addrToCall)() = (void *)(address->startOfData + address->entryPointOffset);
	logBuffer = malloc(32);
	strcpy(logBuffer, "address of entryPoint: 0x");
	utoa((uint64_t)addrToCall, logBuffer + strlen(logBuffer), 16);
	log(MODNAME, logBuffer, LOGLEVEL_DEBUG);
	free(logBuffer);
	log(MODNAME, "ELF Loaded, attempting to launch.", LOGLEVEL_DEBUG);
	addrToCall();
	log(MODNAME, "HOLY CRAP THAT WORKED?!?!?!", LOGLEVEL_DEBUG);
	#endif
	kernTTY.printPrompt();
	kernTTY.blinkingCursor = true;
	kernTTY.cursorAfterPromptX = 0;
	char *command = malloc(512);
	uint16_t commandStrIndex = 0;
	while (true) {
		// Main kernel loop
		char userInput = keyboard.getLastKey();
		if ((uint8_t)userInput == 0xFF) {
			// Special key
			char *specialKey = keyboard.getLastSpecialKey();
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