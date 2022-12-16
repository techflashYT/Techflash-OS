.global putchar
.type putchar, @function
putchar:
	xor %rax, %rax // faster way to zero rax, since putchar is syscall 0
	// we already have the first argument in %rdi, assuming we're following x86_64 SysV ABI (we are, and if we aren't then this isn't the only thing that will break)
	// zero the other arguments registers, putchar doesn't need them for obvious reasons
	xor %rsi, %rsi
	xor %r8, %r8
	// trigger the syscall
	int $0x80
	ret // immediately return with the value