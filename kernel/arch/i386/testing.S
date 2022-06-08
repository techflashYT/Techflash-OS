.global _testasm
.type _testasm, @function
_testasm:
	movl $1, %eax
	add %eax, %eax
	mul %eax
	mul %eax
	mul %eax
	add $182, %eax
	ret 
