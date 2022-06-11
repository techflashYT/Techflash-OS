#include <stdio.h>
#include <stdbool.h>
#include <kernel/arch/i386/pcspeaker.h>
#include <kernel/arch/i386/pit.h>
void panic(const char* message) {
	playSound(1000);
	printf("Kernel panic: %s\r\n", message);
	sleep(2000);
	noSound();
	while (true) {
		asm volatile("cli");
		asm volatile("hlt");
	}
}
