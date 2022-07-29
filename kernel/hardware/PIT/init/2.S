initPIT.l2:
initPIT.gotReloadValue:
	pushq %rax # Store reload value for later
	movw %ax,__PITReloadValue
	movl %eax,%ebx

	movl $3579545,%eax
	movl $0,%edx # 3579545
	divl %ebx # eax = 3579545 / reload value, edx = remainder
	cmp $0x1b4f4c, %edx # Is it more than half?
	jb initPIT.l3 # if not, round down
	incl %eax # if so, round up
