.global __stack_chk_fail
.global stackTrace
.section .bss
backupRAX: .quad 0

.section .text
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

	mov smashedStr, %rdi
	mov regsDump, %rsi
	jmp panic

# NOTE: old, broken, asm stack trace iterations here just for historical record, not actually used
/*
stackTrace:
	# Manual ASM stack trace, since everything I was finding online was i386 only

	# ==================== LAYOUT ====================
	# |	0x00: Pointer to previous stack frame (%RBP) |
	# | 0x08: Return address (%RIP)                  |
	# ================================================

	# Registers in use: 
	# - Can't be trashed by other functions (including us!  We need to save them):
	#	- %rbx: Pointer to allocated memory
	#	- %r12: Current unwound level
	#	- %r13: Backup %rbp
	#	- %r14: Backup %rdi
	#	- %rbp: Current level of stack unwound
	# - We can do whatever, but these will be overwritten by other functions
	#	- %rax: Scratch register for multiplication, also return value
	#	- %r8 : temp register for the address to move %rip into
	#	- %rdi: Parameter: Maximum level to unwind to
	
	# Save all important registers
	push %rbx
	push %r12
	push %r13
	push %r14

	# Save %rdi, we're going to need it later
	mov %rdi, %r14

	# entering stack trace, print debug output
	mov enterStr, %rdi
	call puts

	# Save %rbp
	mov %rbp, %r13

	# save space for final null entry
	inc %rdi
	# multiply by 8 to get size in bytes
	mov $0x08, %rax
	mul %rdi
	mov %rax, %rdi
	call malloc

	# pointer is now in %rbx
	mov %rax, %rbx

.loop:
	cmp %r12, %r14 # if we've hit the max, stop
	je .done

	test %rbp, %rbp # is 0?  if so, stop
	je .done

	addq $0x8, %rbp


	inc %r12
	jmp .loop


.done:
	# null terminate and return

	# restore %rbp
	mov %r13, %rbp

	# pop everything back off the stack, but first, save %rbx as the return value
	mov %rbx, %rax
	pop %r14
	pop %r13
	pop %r12
	pop %rbx
	
	# lets get out of here
	ret
	
*/
/*
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


		# If we're in debug mode, print that we're entering the stack trace
		mov enterStr, %rdi
		call puts
		pop %rdi
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

		# Zero out %rbx and %rcx before we start using them
		xor %rbx, %rbx
		xor %rcx, %rcx

.loop:	
		cmp %rbx, %rdi
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
		
*/