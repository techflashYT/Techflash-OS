initPIT.l3:
	movl $3,%ebx
	movl $0,%edx # 3579545 / reload value
	divl %ebx # (3579545 / 3) / frequency
	cmp $1, %edx # Is it more than half?
	jb initPIT.l4 # if not, round down
	incl %eax # if so, round up
