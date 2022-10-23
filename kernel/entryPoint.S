.section .data
.global kernelStackBottom
.global kernelStackTop
kernelStackBottom:
.skip 0x150000, 0
kernelStackTop:
.section .text

.global _start
.type _start, @function
_start:
	cli
	mov $kernelStackTop, %rsp
	call kernelMain
