.global SSEInit
.global panic
.global FPUInit

.type SSEInit,@function
.type FPUInit,@function

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
	# TODO: Enable SSE2
	ret
SSECheckAdditional:
	# Check for the rest of SSE versions that aren't garunteed with x86_64

	# SSE3
	push %rax
	push %rbx
	push %rcx
	push %rdx
	cpuid
	# Check if bit 0 of ecx is set.
	test $1, %ecx
	jz SSECheckAdditional
	
SSEInit:
	# Since we are running inx x86_64 mode, according to the AMD64 docs, we are garunteed to have at least SSE and SSE2
	call SSEEnable
	mov $0x0, %rax
	ret