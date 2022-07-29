initPIT.l1:
	movl $3,%ebx
	movl $0,%edx # 3579545 * 256 / frequency
	divl %ebx # (3579545 * 256 / 3 * 256) / frequency
	cmp $1, %edx # Is it more than half?
	jb initPIT.l2 # if not, round down
	incl %eax # if so, round up
