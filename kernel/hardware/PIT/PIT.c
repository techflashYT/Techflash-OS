#include <kernel/hardware/PIT.h>
#include <kernel/hardware/CPU/x86Setup.h>
#include <kernel/hardware/IO.h>
#include <stdio.h>
#include <stdlib.h>
#include <kernel/misc.h>
#include <kernel/tty.h>
#include <kernel/custom.h>
#include <kernel/graphics.h>


uint64_t PITTick = 0;
void timerCallback(__attribute__ ((unused)) registers_t *regs) {
	PITTick++;
	
}
// FIXME: Sometimes takes twice as long as it should and I have no idea why
void sleep(uint64_t ms) {
	uint32_t oldTick = 0;
	while (true) {
		if (oldTick != PITTick) {
			oldTick = PITTick;
			ms--;
		}
		if (ms == 0) {
			break;
		}
		asm("hlt");
	}
}
void initPIT(uint32_t frequency) {
	registerInterruptHandler(0x20, &timerCallback); // Register timerCallback on IRQ0

	uint32_t divisor = 1193180 / frequency; // Divide clock to get frequency

	outb(0x43, 0x36); // Send command
	uint8_t l = (uint8_t)(divisor & 0xFF); // Low byte of divisor
	uint8_t h = (uint8_t)((divisor >> 8) & 0xFF); // High byte of divisor
	
	// Send frequency divisor
	outb(0x40, l);
	outb(0x40, h);
}