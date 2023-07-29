#include <stdio.h>
MODULE("X86INT");
void x86_GDT_Init() {
	log(MODNAME, "GDT Initializing", LOGLEVEL_VERBOSE);
}
void x86_IDT_Init() {
	log(MODNAME, "IDT Initializing", LOGLEVEL_VERBOSE);
}