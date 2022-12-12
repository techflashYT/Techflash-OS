#include <kernel/tty.h>
#include <kernel/hardware/kbd.h>
#include <kernel/hardware/serial.h>
#include <kernel/environment.h>
#include <kernel/boot.h>
#include <kernel/graphics.h>
#include <kernel/hardware/CPU/regs.h>
#include <kernel/font.h>
#include <kernel/elf.h>
#include <stdint.h>
#include <stdlib.h>

elfLoader_t elfLoader;
extern uint8_t elfCheckIsValid(uint8_t *header, uint8_t arch);
extern elfStruct_t *elfLoad(uint8_t *file);

registers_t regsDump;
extern void kernTTY_init();
extern void kernTTY_setBackground(const uint32_t color);
extern void kernTTY_printPrompt();
extern void kernTTY_blink();
extern void kernTTY_scroll(const char *numLines);
_kernTTY_t kernTTY;

extern bool __keyboardGetStatusOfLED(const uint8_t led);
extern void __keyboardSetLED(const uint8_t led, const bool value);
keyboard_t keyboard;
extern bool __serialInit();
extern char __serialReadNext(const uint16_t port);
extern void __serialWrite(const uint16_t port, const char value);
extern void __serialWriteString(const uint16_t port, const char* value);
__serial_t serial;

__environment_t env;

extern void bootProgressBarUpdate(const uint8_t percent);
extern void bootProgressBarCreate(const uint8_t x, const uint8_t y, const uint8_t width);
extern void bootProgressBarFadeOut();
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
vga_t vga;

bool heapSetUp;
bool haveAllocated;
unsigned char *lastValidHeapAddress;
unsigned char *heapSpace;

psf2_t *font;
extern char *arguments;
bool timerReady;
void __initThings() {
	elfLoader.isValid        = elfCheckIsValid;
	elfLoader.load			 = elfLoad;
	timerReady = false;
	font = (psf2_t*)&_binary_font_psf_start;

	boot.percent = 0;
	boot.progressBar.create  = bootProgressBarCreate;
	boot.progressBar.update  = bootProgressBarUpdate;
	boot.progressBar.fadeOut = bootProgressBarFadeOut;
	// Start setting up the serial struct.
	serial.init              = __serialInit;
	serial.readNext          = __serialReadNext;
	serial.write             = __serialWrite;
	serial.writeString       = __serialWriteString;
	// serial.init();


	vga.colors.black         = __VGAColorBlack;
	vga.colors.blue          = __VGAColorBlue;
	vga.colors.green         = __VGAColorGreen;
	vga.colors.cyan          = __VGAColorCyan;
	vga.colors.red           = __VGAColorRed;
	vga.colors.purple        = __VGAColorPurple;
	vga.colors.brown         = __VGAColorBrown;
	vga.colors.lgray         = __VGAColorLGray;
	vga.colors.dgray         = __VGAColorDGray;
	vga.colors.lblue         = __VGAColorLBlue;
	vga.colors.lgreen        = __VGAColorLGreen;
	vga.colors.lcyan         = __VGAColorLCyan;
	vga.colors.lred          = __VGAColorLRed;
	vga.colors.pink          = __VGAColorPink;
	vga.colors.yellow        = __VGAColorYellow;
	vga.colors.white         = __VGAColorWhite;
	
	// Start initializing a TTY.
	kernTTY.init             = kernTTY_init;
	kernTTY.setBackground    = kernTTY_setBackground;
	kernTTY.printPrompt      = kernTTY_printPrompt;
	kernTTY.blink            = kernTTY_blink;
	kernTTY.scroll           = kernTTY_scroll;
	kernTTY.init();

	// Start setting up the keyboard struct.
	keyboard.getStatusLED    = __keyboardGetStatusOfLED;
	keyboard.setLED          = __keyboardSetLED;


	arguments = malloc(128);
}