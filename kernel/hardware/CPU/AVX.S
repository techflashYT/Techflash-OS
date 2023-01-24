.global AVXInit
.type AVXInit, @function

AVXInit:
	push %rcx
	push %rdx

	xor %rcx, %rcx
	xgetbv
	or $7, %eax
	xsetbv

	pop %rdx
	pop %rcx
	xor %rax, %rax
	ret
