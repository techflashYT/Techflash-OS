static inline uint_fast32_t farpeekl(uint_fast16_t sel, void* off) {
	uint_fast32_t ret;
	asm (	"push %%fs\n\t"
			"mov  %1, %%fs\n\t"
			"mov  %%fs:(%2), %0\n\t"
			"pop  %%fs"
			: "=r"(ret) : "g"(sel), "r"(off) );
	return ret;
}

static inline void farpokeb(uint_fast16_t sel, void* off, uint_fast8_t v) {
	asm (	"push %%fs\n\t"
			"mov  %0, %%fs\n\t"
			"movb %2, %%fs:(%1)\n\t"
			"pop %%fs"
			: : "g"(sel), "r"(off), "r"(v));
	/* TODO: Should "memory" be in the clobber list here? */
}

static inline void outb(uint_fast16_t port, uint_fast8_t val) {
	asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
	/* There's an outb %al, $imm8  encoding, for compile-time constant port numbers that fit in 8b.  (N constraint).
	 * Wider immediate constants would be truncated at assemble-time (e.g. "i" constraint).
	 * The  outb  %al, %dx  encoding is the only option for all other cases.
	 * %1 expands to %dx because  port  is a uint16_t.  %w1 could be used if we had the port number a wider C type */
}

static inline uint_fast8_t inb(uint_fast16_t port) {
	uint_fast8_t ret;
	asm volatile ( "inb %1, %0"
				   : "=a"(ret)
				   : "Nd"(port) );
	return ret;
}

