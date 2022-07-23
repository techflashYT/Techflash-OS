#include <stdio.h>
void panic(const char* message) {
	asm volatile("xchgw %bx, %bx");
	printf("KERNEL PANIC: %s\n", message);
	asm volatile("cli\n" "hlt");
}