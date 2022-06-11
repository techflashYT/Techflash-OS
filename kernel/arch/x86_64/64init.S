.global _checkForLongModeCPUID
.type _checkForLongModeCPUID, @function
.global _noLongMode
.type _noLongMode, @function
.global _enablePaging
.type _enablePaging, @function
.global _goLong
.type _goLong, @function
_checkForLongModeCPUID:
	pushal
	# Check for CPUID instruction
	pushf
	pop %eax
	mov %eax, %ecx
	xor $0x200000, %eax
	push %eax
	popf

	pushf
	pop %eax
	push %ecx
	popf

	xor %ecx, %eax
	je _noCPUID
	# We have CPUID, check if we can check for long mode
	mov $0x80000000, %eax
	cpuid
	cmp $0x80000001, %eax
	jb _noExtCPUID
	# We have the ability to check if long mode is available
	mov $0x80000001, %eax
	cpuid
	test $0x20000000, %edx
	jz _noLongMode
	# Long mode is availible
	popal
	ret
errorMessage_noCPUID:
	.asciz "Your CPU does not support the `cpuid` instruction.  Please run this on a newer PC."
errorMessage_noLongMode:
	.asciz "Your CPU is not 64-bit!\r\nIf you want to run this on a 32-bit computer, please use the 32-bit version!"
errorMessage_noExtCPUID:
	.asciz "Your CPU is too old for this OS!  Please use a newer CPU that supports extended CPUID!"
_noExtCPUID:
	call terminalInit
	push $errorMessage_noExtCPUID
	call panic
_noLongMode:
	call terminalInit
	push $errorMessage_noLongMode
	call panic
_noCPUID:
	call terminalInit
	push $errorMessage_noCPUID
	call panic

_enablePaging:
	# Set up paging
	mov %cr0, %eax
	and $0x7FFFFFFF, %eax
	mov %eax, %cr0

	# Enable paging

	# Clear tables
	mov $0x1000, %edi
	mov %edi, %cr3
	xor %eax, %eax
	mov $0x1000, %ecx
	rep stos %eax, %es:(%edi)
	mov %cr3, %edi

	# PML4T[0] -> PDPT
	movl $0x2003, (%edi)
	add $0x1000, %edi
	movl $0x3003, (%edi)
	add $0x1000, %edi
	movl $0x4003, (%edi)
	add $0x1000, %edi


	# Identity map the first 2MB
	mov $0x3, %ebx
	mov $0x200, %ecx
.SetEntry:
	mov %ebx, (%edi)
	add $0x1000, %ebx
	add $0x8, %edi
	loop .SetEntry
	ret

_goLong:
	# PAE
	xchgw %bx, %bx
	mov %cr4, %eax
	or $0x20, %eax
	mov %eax, %cr4

	# Really enable paging
	mov %cr0, %eax
	or $0x80000000, %eax
	mov %eax, %cr0

	ret
