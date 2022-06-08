.global _testasm
.type _testasm, @function
_testasm:
	push %edx
	mov $1234, %edx
	imul $59424, %edx
	movl %edx, %eax
	pop %edx
	ret 
