#include <kernel/hardware/PIT.h>
#include <kernel/hardware/CPU/regs.h>
#include <kernel/hardware/CPU/ISR.h>
#include <kernel/hardware/CPU/IRQNums.h>
#include <kernel/hardware/IO.h>
#include <stdio.h>
#include <stdlib.h>
#include <kernel/misc.h>
uint32_t tick = 0;
static void timerCallback(registers_t *regs) {
	tick++;
	char *buffer = "\0\0\0\0";
	itoa(tick, buffer, 10);
	puts("PIT Tick!: ");
	puts(buffer);
	puts("\r\n");
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