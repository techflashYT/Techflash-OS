# TODO: absolutely horrible, fix this mess
.global SSEInit
.global panic
.global FPUInit

.type SSEInit,@function
.type FPUInit,@function

.section .bss
SSSE3:
.byte 0
.section .text
SSEEnable:
	# Enable SSE
	mov %cr0, %rax
	and $0xFFFB, %ax # Clear coprocessor emulation bit (use hardware FPU)
	push %rax
	call FPUInit
	pop %rax
	mov %rax, %cr0
	mov %cr4, %rax
	or $3 << 9, %ax
	mov %rax, %cr4
SSECheckAdditional:
	# Check for the rest of SSE versions that aren't garunteed with x86_64

	push %rbx
	push %rcx
	push %rdx
	cpuid
	# SSE3
	# Check if bit 0 of ecx is set.
	test $1, %ecx
	jz .NoSSE3
	# SSSE3
	# Check if bit 9 of ecx is set.
	test $1 << 9, %ecx
	jnz .YesSSSE3
	.part2:
		# SSE4.1
		# Check if bit 19 of ecx is set.
		test $1 << 19, %ecx
		jz .NoSSE41
		# SSE4.2
		# Check if bit 20 of ecx is set.
		test $1 << 20, %ecx
		jz .NoSSE42
		# SSE4A
		# Check if bit 6 of ecx is set.
		test $1 << 6, %ecx
		jz .NoSSE4A
		# We don't bother with SSE5 because it's still new enough that I don't want to enable support for it in the kernel.
		# Bit 0: SSE1
		# Bit 1: SSE2
		# Bit 2: SSE3
		# Bit 3: SSSE3
		# Bit 4: SSE4.1
		# Bit 5: SSE4.2
		# Bit 6: SSE4A
		# Bit 7: Unused
		mov 0b01110111, %rax
	.retrn:
		testb $1, SSSE3
		jz .YesSSSE3p2
	.realretrn:
		pop %rdx
		pop %rcx
		pop %rbx
		ret
	.NoSSE3:
		mov 0b00000011, %rax
		jmp .retrn
	.YesSSSE3:
		movb $1, SSSE3
		jz .part2
	.YesSSSE3p2:
		# Set bit 3 to 1 without affecting other bits.
		or $1 << 3, %rax
		jmp .realretrn
	.NoSSE41:
		mov 0b00000111, %rax
		jmp .retrn
	.NoSSE42:
		mov 0b00010111, %rax
		jmp .retrn
	.NoSSE4A:
		mov 0b00110111, %rax
		jmp .retrn
SSEInit:
	# Since we are running in x86_64 mode, according to the AMD64 docs, we are garunteed to have at least SSE and SSE2
	call SSEEnable # Calls SSECheckAdditional by simply continuing down.
	ret