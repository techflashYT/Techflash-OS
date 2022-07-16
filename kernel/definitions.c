#include <stdint.h>
#include <kernel/tty.h>
#include <kernel/hardware/kbd.h>
#include <kernel/hardware/serial.h>

extern void __kernTTY_init();
_kernTTY_t kernTTY;

extern bool __keyboardGetStatusOfLED(uint8_t led);
extern void __keyboardSetLED(uint8_t led, bool value);
__keyboard_t keyboard;

extern int __serialInit();
extern char __serialReadNext(uint16_t port);
extern void __serialWrite(uint16_t port, char value);
__serial_t serial;
void __init() {
	__asm__ ("cli\n""hlt");
	// Start initializing a TTY.
	kernTTY.init = __kernTTY_init;
	kernTTY.init();

	// Start setting up the keyboard struct.
	keyboard.getStatusLED = __keyboardGetStatusOfLED;
	keyboard.setLED = __keyboardSetLED;

	// Start setting up the serial struct.
	serial.init = __serialInit;
	serial.readNext = __serialReadNext;
	serial.write = __serialWrite;
}