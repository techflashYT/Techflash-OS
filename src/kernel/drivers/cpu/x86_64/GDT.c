#include "../../../../shared.h"
#include "../../../../cstdlib/defs.h"

typedef struct tssEntry {
	uint32_t	prev_tss;
	uint32_t	esp0;
	uint32_t	ss0;
	uint32_t	esp1;
	uint32_t	ss1;
	uint32_t	esp2;
	uint32_t	ss2;
	uint32_t	cr3;
	uint32_t	eip;
	uint32_t	eflags;
	uint32_t	eax;
	uint32_t	ecx;
	uint32_t	edx;
	uint32_t	ebx;
	uint32_t	esp;
	uint32_t	ebp;
	uint32_t	esi;
	uint32_t	edi;
	uint32_t	es;
	uint32_t	cs;
	uint32_t	ss;
	uint32_t	ds;
	uint32_t	fs;
	uint32_t	gs;
	uint32_t	ldt;
	uint16_t	trap;
	uint16_t	iomap_base;
} __attribute__ ((packed)) tss_entry_t;

static void writeTss(int32_t, uint16_t, uint32_t);
tss_entry_t tss_entry;

struct gdtEntry {
	/* Limits */
	unsigned short limit_low;
	/* Segment address */
	unsigned short base_low;
	unsigned char base_middle;
	/* Access modes */
	unsigned char access;
	unsigned char granularity;
	unsigned char base_high;
} __attribute__((packed));


struct gdtPtr {
	unsigned short limit;
	unsigned int base;
} __attribute__((packed));

struct gdtEntry	gdt[6];
struct gdtPtr	gp;


extern void gdtFlush();


void gdtSetGate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran) {
	/* Base Address */
	gdt[num].base_low     =	 (base & 0xFFFF);
	gdt[num].base_middle  =  (base >> 16) & 0xFF;
	gdt[num].base_high    =	 (base >> 24) & 0xFF;
	/* Limits */
	gdt[num].limit_low    =	 (limit & 0xFFFF);
	gdt[num].granularity  =  (limit >> 16) & 0X0F;
	/* Granularity */
	gdt[num].granularity  |= (gran & 0xF0);
	/* Access flags */
	gdt[num].access = access;
}

void gdtInstall() {
	/* GDT pointer and limits */
	gp.limit = (sizeof(struct gdtEntry) * 6) - 1;
	gp.base = (unsigned int)&gdt;
	/* NULL */
	gdtSetGate(0, 0, 0, 0, 0);
	/* Code segment */
	gdtSetGate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
	/* Data segment */
	gdtSetGate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
	/* User code */
	gdtSetGate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
	/* User data */
	gdtSetGate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);
	writeTss(5, 0x10, 0x0);
	/* Go go go */
	gdtFlush();
	tssFlush();
}

static void writeTss(int32_t num, uint16_t ss0, uint32_t esp0) {
	uintptr_t base  = (uintptr_t)&tss_entry;
	uintptr_t limit = base + sizeof(tss_entry);

	/* Add the TSS descriptor to the GDT */
	gdt_set_gate(num, base, limit, 0xE9, 0x00);

	memset(&tss_entry, 0x0, sizeof(tss_entry));

	tss_entry.ss0    = ss0;
	tss_entry.esp0   = esp0;
	/* Zero out the descriptors */
	tss_entry.cs     = 0x0b;
	tss_entry.ss     = tss_entry.ds = tss_entry.es = tss_entry.fs =	tss_entry.gs = 0x13;
	tss_entry.iomap_base = sizeof(tss_entry);
}
void setKernelStack(uintptr_t stack) {
	tss_entry.esp0 = stack;
}
