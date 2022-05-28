.global _testasm
.type _testasm, @function
_testasm:
	movl $0x12345678, %eax
	ret 
