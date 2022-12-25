#include <kernel/tty.h>
#include <kernel/hardware/kbd.h>
#include <kernel/hardware/serial.h>
#include <kernel/hardware/parallel.h>
#include <kernel/environment.h>
#include <kernel/boot.h>
#include <kernel/graphics.h>
#include <kernel/hardware/CPU/regs.h>
#include <kernel/bda.h>
#include <kernel/font.h>
#include <kernel/elf.h>
#include <stdint.h>
#include <stdlib.h>

extern void elfInit();
registers_t regsDump;

extern void kernTTY_init();
extern void keyboardInit();
extern bool serialInit(uint64_t speed);
extern void bootInit();
extern void VGAInit();
extern void parallelInit();
bda_t *bda;
bool heapSetUp;
bool haveAllocated;
unsigned char *lastValidHeapAddress;
unsigned char *heapSpace;

psf2_t *font;
extern char *arguments;
bool timerReady;
// calls the init functions to initialize the function pointers for all of the structs
void initThings() {
	bda = (void *)0x400;
	elfInit();
	timerReady = false;
	font = (psf2_t*)&_binary_font_psf_start;

	// initialize the boot struct.
	bootInit();

	// Start setting up the serial struct.
	serialInit(0);

	// initialize the parallel struct.
	parallelInit();

	// initialize the vga struct
	VGAInit();
	
	// Start initializing a TTY.
	kernTTY_init();

	// Start setting up the keyboard struct.
	keyboardInit();

	arguments = malloc(128);
}