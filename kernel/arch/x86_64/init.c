#include <kernel/arch/x86/GDT_IDT.h>
extern void x86_Exceptions_Init();
void x86_64_Init() {
	x86_GDT_Init();
	x86_IDT_Init();
	x86_Exceptions_Init();
	return;
}