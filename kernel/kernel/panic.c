#include <stdio.h>
#include <stdbool.h>
void panic(const char* message) {
	printf("Kernel panic: %s\r\n", message);
	while (true) {
		asm volatile("cli");
		asm volatile("hlt");
	}
}
