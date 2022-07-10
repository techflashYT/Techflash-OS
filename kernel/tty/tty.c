#include <kernel/tty.h>
#include <stdbool.h>
void __kernTTY_init() {
	kernTTY.ready = false;
	kernTTY.cursorX = 0;
	kernTTY.cursorY = 0;
	kernTTY.color = 0xAAAAAA; // VGA light gray.
}