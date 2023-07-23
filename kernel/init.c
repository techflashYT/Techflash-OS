#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <kernel/font.h>
#include <kernel/bda.h>
#include <kernel/tty.h>
#include <kernel/environment.h>
#include <kernel/panic.h>
#include <kernel/hardware/serial.h>
#include <kernel/hardware/CPU/x86Setup.h>

extern void elfInit();
registers_t regsDump;

extern bool serialInit(uint64_t speed);
extern void colorsInit();
extern void parallelInit();
bda_t *bda;
bool heapSetUp;
bool haveAllocated;
unsigned char *lastValidHeapAddress;
unsigned char *heapSpace;

// psf2_t *font;
bool timerReady;

volatile struct limine_hhdm_request hhdm = {
	.id = LIMINE_HHDM_REQUEST,
	.revision = 0
};
volatile struct limine_framebuffer_request limineFb = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};
volatile struct limine_framebuffer *fb;

// calls the init functions to initialize the function pointers for all of the structs
void initThings() {
	// font = (psf2_t*)&_binary_font_psf_start;
	bda = (void *)0x400 + hhdm.response->offset;
	// elfInit();
	timerReady = false;
	// memset(((uint32_t *)fb->address), 0x0000FFff, 1000);
	// ((uint32_t *)fb->address)[0] = 0xffffffff;
	// ((uint32_t *)fb->address)[1] = 0xffffffff;
	// ((uint32_t *)fb->address)[2] = 0xffffffff;
	// ((uint32_t *)fb->address)[3] = 0xffffffff;

	

	// Init serial logging
	serialInit(115200);

	// initialize the parallel struct.
	parallelInit();

	// initialize the colors struct
	colorsInit();
	
	char str[64];
	for (uint_fast8_t i = 0; i != limineFb.response->framebuffer_count; i++) {
		sprintf(str, "fb[%d]: %p\r\n", i, limineFb.response->framebuffers);
		serial.writeString(SERIAL_PORT_COM1, str);
	}

	fb = limineFb.response->framebuffers[0];

	// Start initializing a TTY.
	TTY_Init();
	// DUMPREGS;
	// panic("test123", regs);
}