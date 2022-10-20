#include <kernel/hardware/PIT.h>
#include <kernel/hardware/CPU/regs.h>
#include <kernel/hardware/CPU/ISR.h>
#include <kernel/hardware/CPU/IRQNums.h>
#include <kernel/hardware/IO.h>
#include <stdio.h>
#include <stdlib.h>
#include <kernel/misc.h>
#include <kernel/tty.h>
#include <kernel/custom.h>
#include <kernel/graphics.h>


uint32_t PITTick = 0;
void timerCallback(registers_t *regs) {
	PITTick++;
	kernTTY.blink();
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
	// Register timer callback
	registerInterruptHandler(IRQ0, &timerCallback);
	// Divide clock to get frequency
	uint32_t divisor = 1193180 / frequency;
	// Send command
	outb(0x43, 0x36);
	// Low byte of divisor
	uint8_t l = (uint8_t)(divisor & 0xFF);
	// High byte of divisor
	uint8_t h = (uint8_t)((divisor >> 8) & 0xFF);
	// Send frequency divisor
	outb(0x40, l);
	outb(0x40, h);
}