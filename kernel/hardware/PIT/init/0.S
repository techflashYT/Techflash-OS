.global initPIT
.type initPIT, @function
initPIT:
	mov %esp, __temporaryStackPointer
	push %rax
	push %rcx
	push %rdx
	push %rbx
	push (__temporaryStackPointer)
	push %rbp
	push %rsi
	push %rdi
	
	movl $0x10000,%eax # reload value to 65536
	cmpl $18,%ebx # is it too slow?
	jbe initPIT.gotReloadValue # if so, set it to the slowest possible value

	movl $1,%eax # reload value for fastest frequency
	cmpl $1193181,%ebx # is it too fast?
	jae initPIT.gotReloadValue # if so, set it to the fastest possible value

	movl $3579545,%eax
	movl $0,%edx
	divl %ebx # eax = 3579545 / frequency, edx = remainder
	cmp $0x1b4f4c, %edx # Is it more than half?
	jb initPIT.l1 # if not, round down
	incl %eax # if so, round up
