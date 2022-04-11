#include "../../../../inc/util.hpp"
#include <cstdlib>
#include <cstdint>
void idtSetGate(uint8_t num, uint64_t base, uint16_t sel, uint8_t flags);


extern void _irq0();
extern void _irq1();
extern void _irq2();
extern void _irq3();
extern void _irq4();
extern void _irq5();
extern void _irq6();
extern void _irq7();
extern void _irq8();
extern void _irq9();
extern void _irq10();
extern void _irq11();
extern void _irq12();
extern void _irq13();
extern void _irq14();
extern void _irq15();

/* Registers */
struct regs {
	uint32_t gs, fs, es, ds;
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
	uint32_t int_no, err_code;
	uint32_t eip, cs, eflags, useresp, ss;
};

typedef struct regs regs_t;

typedef void (*irq_handler_t) (struct regs *);

static irq_handler_t irq_routines[16] = { NULL };

/*
 * Install an interupt handler for a hardware device.
 */
void irq_install_handler(int8_t irq, irq_handler_t handler) {
	irq_routines[irq] = handler;
}

/*
 * Remove an interrupt handler for a hardware device.
 */
void irq_uninstall_handler(int irq) {
	irq_routines[irq] = 0;
}

/*
 * Remap interrupt handlers
 */
void irq_remap() {
	outb(0x20, 0x11);
	outb(0xA0, 0x11);
	outb(0x21, 0x20);
	outb(0xA1, 0x28);
	outb(0x21, 0x04);
	outb(0xA1, 0x02);
	outb(0x21, 0x01);
	outb(0xA1, 0x01);
	outb(0x21, 0x0);
	outb(0xA1, 0x0);
}

void irq_gates() {
	idt_set_gate(32, (unsigned)_irq0, 0x08, 0x8E);
	idt_set_gate(33, (unsigned)_irq1, 0x08, 0x8E);
	idt_set_gate(34, (unsigned)_irq2, 0x08, 0x8E);
	idt_set_gate(35, (unsigned)_irq3, 0x08, 0x8E);
	idt_set_gate(36, (unsigned)_irq4, 0x08, 0x8E);
	idt_set_gate(37, (unsigned)_irq5, 0x08, 0x8E);
	idt_set_gate(38, (unsigned)_irq6, 0x08, 0x8E);
	idt_set_gate(39, (unsigned)_irq7, 0x08, 0x8E);
	idt_set_gate(40, (unsigned)_irq8, 0x08, 0x8E);
	idt_set_gate(41, (unsigned)_irq9, 0x08, 0x8E);
	idt_set_gate(42, (unsigned)_irq10, 0x08, 0x8E);
	idt_set_gate(43, (unsigned)_irq11, 0x08, 0x8E);
	idt_set_gate(44, (unsigned)_irq12, 0x08, 0x8E);
	idt_set_gate(45, (unsigned)_irq13, 0x08, 0x8E);
	idt_set_gate(46, (unsigned)_irq14, 0x08, 0x8E);
	idt_set_gate(47, (unsigned)_irq15, 0x08, 0x8E);
}

/*
 * Set up interrupt handler for hardware devices.
 */
void irq_install() {
	irq_remap();
	irq_gates();
	IRQ_RES;
}

void irq_ack(int irq_no) {
	if (irq_no >= 12) {
		outb(0xA0, 0x20);
	}
	outb(0x20, 0x20);
}

void
irq_handler(struct regs *r) {
	IRQ_OFF;
	void (*handler)(struct regs *r);
	if (r->int_no > 47 || r->int_no < 32) {
		handler = NULL;
	}
	else {
		handler = irq_routines[r->int_no - 32];
	}
	if (handler) {
		handler(r);
	}
	else {
		irq_ack(r->int_no - 32);
	}
	IRQ_RES;
}
