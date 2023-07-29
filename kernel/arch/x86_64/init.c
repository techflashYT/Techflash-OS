#include <kernel/arch/x86/GDT_IDT.h>
void x86_64_Init() {
	x86_GDT_Init();
	x86_IDT_Init();
}