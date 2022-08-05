.global AVXInit
.type AVXInit, @function

AVXInit:
	push %rax
	push %rcx
	push %rdx

	xor %rcx, %rcx
	xgetbv
	or $7, %eax
	xsetbv

	pop %rdx
	pop %rcx
	pop %rax
	ret
