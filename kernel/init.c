#include <kernel/tty.h>
#include <kernel/hardware/kbd.h>
#include <kernel/hardware/serial.h>
#include <kernel/environment.h>
#include <kernel/boot.h>
#include <kernel/graphics.h>
#include <kernel/hardware/CPU/regs.h>

#include <stdint.h>

registers_t regsDump;
extern void __kernTTY_init();
extern void __kernTTY_setBackground(const uint32_t color);
extern void __kernTTY_printPrompt();
extern void __kernTTY_blink();
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
extern void __bootProgressBarFadeOut();
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

bool heapSetUp;
bool haveAllocated;
unsigned char *lastValidHeapAddress;
unsigned char *heapSpace;

extern char *arguments;
void __initThings() {
	boot.percent = 0;
	boot.progressBar.create  = __bootProgressBarCreate;
	boot.progressBar.update  = __bootProgressBarUpdate;
	boot.progressBar.fadeOut = __bootProgressBarFadeOut;
	// Start setting up the serial struct.
	serial.readNext = __serialReadNext;
	serial.write = __serialWrite;
	serial.writeString = __serialWriteString;

	// Start initializing a TTY.
	kernTTY.init = __kernTTY_init;
	kernTTY.setBackground = __kernTTY_setBackground;
	kernTTY.printPrompt = __kernTTY_printPrompt;
	kernTTY.blink = __kernTTY_blink;
	kernTTY.init();

	// Start setting up the keyboard struct.
	keyboard.getStatusLED = __keyboardGetStatusOfLED;
	keyboard.setLED = __keyboardSetLED;

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

	arguments = malloc(128);
}