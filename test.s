	.file	"main.cpp"
	.text
	.p2align 4
	.globl	_Z4haltv
	.type	_Z4haltv, @function
_Z4haltv:
.LFB46:
	.cfi_startproc
	.p2align 4,,10
	.p2align 3
.L2:
#APP
# 3 "src/kernel/../inc/util.hpp" 1
	hlt
# 0 "" 2
#NO_APP
	jmp	.L2
	.cfi_endproc
.LFE46:
	.size	_Z4haltv, .-_Z4haltv
	.p2align 4
	.globl	_Z5printPKhRS_
	.type	_Z5printPKhRS_, @function
_Z5printPKhRS_:
.LFB47:
	.cfi_startproc
	movzbl	(%rdi), %edx
	testb	%dl, %dl
	je	.L4
	movq	vga_buffer(%rip), %rax
	xorl	%r8d, %r8d
	leaq	4(%rax), %rcx
	.p2align 4,,10
	.p2align 3
.L6:
	movzbl	(%rsi), %eax
	addl	$1, %r8d
	movq	%rcx, vga_buffer(%rip)
	addq	$4, %rcx
	sall	$8, %eax
	orl	%edx, %eax
	movl	%eax, -8(%rcx)
	movzbl	%r8b, %eax
	movzbl	(%rdi,%rax), %edx
	testb	%dl, %dl
	jne	.L6
.L4:
	ret
	.cfi_endproc
.LFE47:
	.size	_Z5printPKhRS_, .-_Z5printPKhRS_
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"ABC"
	.text
	.p2align 4
	.globl	kernelMain
	.type	kernelMain, @function
kernelMain:
.LFB48:
	.cfi_startproc
	movq	vga_buffer(%rip), %rax
	xorl	%esi, %esi
	movl	$65, %ecx
	leaq	.LC0(%rip), %rdi
	.p2align 4,,10
	.p2align 3
.L10:
	addl	$1, %esi
	movzbl	%cl, %edx
	movq	%rax, %r8
	addq	$4, %rax
	movzbl	%sil, %ecx
	orb	$15, %dh
	movzbl	(%rdi,%rcx), %ecx
	movl	%edx, -4(%rax)
	testb	%cl, %cl
	jne	.L10
	movq	%rax, vga_buffer(%rip)
	movl	%edx, (%r8)
	.p2align 4,,10
	.p2align 3
.L11:
#APP
# 3 "src/kernel/../inc/util.hpp" 1
	hlt
# 0 "" 2
#NO_APP
	jmp	.L11
	.cfi_endproc
.LFE48:
	.size	kernelMain, .-kernelMain
	.globl	vga_buffer
	.data
	.align 8
	.type	vga_buffer, @object
	.size	vga_buffer, 8
vga_buffer:
	.quad	753664
	.ident	"GCC: (Debian 11.2.0-19) 11.2.0"
	.section	.note.GNU-stack,"",@progbits
