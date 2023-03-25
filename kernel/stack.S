overflowStr:
.asciz "Kernel Stack Overflow (this stack trace trace might be bogus!)"
smashedStr:
.asciz "Stack smashing detected (this stack trace might be bogus!)"
.global __stack_chk_guard
.global __stack_chk_fail
.global stackTrace
__stack_chk_guard:
#if defined(__X86_64__) || defined(_M_X64) || defined (__aarch64__) || defined(_M_ARM64)
.quad 0x595e9fbd94fda766
#else
.long 0xe2dee396
#endif
backupRAX:
.quad 0
__stack_chk_fail:
	movq %rax, backupRAX
	movq regsDump, %rax
	
	movq %rdi,           0(%rax)
	movq %rsi,           8(%rax)
	movq %rbp,          16(%rax)
	movq %rbx,          24(%rax)
	movq %rdx,          32(%rax)
	movq %rcx,          40(%rax)
	movq $backupRAX,    48(%rax)
	leaq 0x0(%rip),        %rbx
	mov  %rbx,		   128(%rax)

	mov kernelStackTop, %rax
	cmpb $0xAD, 1(%rax)
	jne .stackOverflow
	cmpb $0xDE, 0(%rax)
	jne .stackOverflow
	jmp .bye
	.stackOverflow:
		mov (emergencyStack), %rsp
		mov overflowStr, %rdi
		mov regsDump, %rsi
		jmp panic
	.bye: // didn't overflow, but corrupted anyways
		mov smashedStr, %rdi
		mov regsDump, %rsi
		jmp panic
stackTrace:
		# Manual ASM stack trace, since everything I was finding online was i386 only

		# ==================== LAYOUT ====================
		# |	0x00: Pointer to previous stack frame (%RBP) |
		# | 0x08: Return address (%RIP)                  |
		# ================================================

		# Registers in use: 
		#	- %rax: Pointer to allocated memory
		#	- %rbx: Current unwound level
		#	- %rcx: offset into memory
		#	- %rdx: Backup %rbp
		#	- %r8 : temp register for the address to move %rip into
		#	- %rbp: Current level of stack unwound
		#	- %rdi: Parameter: Maximum level to unwind to
		
		# Save %rdi, we're going to need it later
		push %rdi

		# Calculate how much memory we need for the data to return
		#! malloc((max + 1) * 8)

		inc %rdi # Save some space for the size at the beginning

		# Clobbers RDX:RAX!
		mov $0x08, %rax # Multiply by 8
		mul %rdi        # %rdx:%rax = %rdi * 8
		mov %rax, %rdi  # Put it in the first arg

		# We now have the number of bytes of memory that we need to allocate.
		call malloc # Allocate

		mov %rbp, %rdx # Save %rbp in %rdx

		pop %rdi # Retrieve %rdi

		# Zero out %rcx before we start using it
		xor %rcx, %rcx

.loop:	cmp %rbx, %rdi
		je .done # have we hit the max?  If so, we're done
		
		cmpq $0x00, (%rbp) # if we hit a null entry, we're done
		je .done

		addq $0x8, %rcx

		# compute the address to move the saved %rip into
		mov %rdx, %r8
		add %rcx, %r8
		# %r8 is now the address of the next free place in the struct

		# copy %rip into the memory
		movq (%rbp), %r9
		movq %r9, (%r8)

		inc %rbx
		jmp .loop # repeat loop

.done:	mov %rdx, %rbp   # restore %rbp
		mov %rbx, (%rdx) # size
		ret
		
