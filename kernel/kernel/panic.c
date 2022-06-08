#include <stdio.h>
#include <stdbool.h>
#include <kernel/arch/i386/pcspeaker.h>
void panic(const char* message) {
	playSound(1000);
	printf("Kernel panic: %s\r\n", message);
	// TODO: Use Programmable Interval timer to wait about 2 seconds
	noSound();
	while (true) {
		asm volatile("cli");
		asm volatile("hlt");
	}
}
