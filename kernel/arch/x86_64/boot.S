#include "multiboot2header.inc"
# Reserve a stack for the initial thread.
.global stackTop
.section .bss
.align 16
stackBottom:
.skip 131072 # 128K of stack
stackTop:
# The kernel entry point.
.section .text
.global _start
.type _start, @function

_start:
	cli
	movl $stackTop, %esp
	pushl %ebx
	pushl %eax
	call _bootmsg

	# Call the global constructors.
	call _init
	movb $0xFE, 56(%edx) # .
	movb $0x06, 57(%edx) # brown (orange)
	# Initialize the floating point unit.
	finit
	movb $0xFE, 58(%edx) # .
	movb $0x0E, 59(%edx) # yellow
	# TODO: Initialize the PIC and PIT
	
	
	# Check if A20 Gate is already enabled from grub 
	pushal
	mov $0x112345, %edi  # odd megabyte address.
	mov $0x012345, %esi  # even megabyte address.
	mov %esi, (%esi)     # making sure that both addresses contain diffrent values.
	mov %esi, (%esi)     # (if A20 line is cleared the two pointers would point to the address 0x012345 that would contain 0x112345 (edi)) 
	cmpsd             # compare addresses to see if the're equivalent.
	popal
	movb $0xFE, 60(%edx) # .
	movb $0x0A, 61(%edx) # lightgreen
	pushal
	jne __bootstg2 # if A20 is on, jmp to call boot stage2.
	popal
	# Enable A20 Gate if it's not enabled.
	in $0x92, %al
	or $2, %al
	out %al, $0x92
	jmp __bootstg2

nofpu:
	mov $0x0, %eax
	ret

__bootstg2:
	movb $0xFE, 62(%edx) # .
	movb $0x02, 63(%edx) # green
	call _disableCursor
	call _checkForLongModeCPUID # Call a function that checks if we have 64-bit long mode availible by using the `cpuid` instruction
	movb $0xFE, 64(%edx) # .
	movb $0x01, 65(%edx) # blue
	pushal
	call IDTinit
	popal
	# Just in case, lets zero out the stack
	call _enablePaging
	movb $0xFE, 66(%edx) # .
	movb $0x09, 67(%edx) # lightblue
	call _goLong
	# Just in case interrupts are enabled, disable them.
	cli
	# Also just in case, clear all the regs
	xor %eax, %eax
	xor %ebx, %ebx
	xor %ecx, %ecx
	xor %edx, %edx
	xor %esi, %esi
	xor %edi, %edi
	xor %ebp, %ebp
	# Transfer control to the main kernel.
	movb $0xFE, 68(%edx) # .
	movb $0x03, 69(%edx) # yellow
	call kernelMain

	# Hang if kernelMain unexpectedly returns.
	cli
	mov $0xFE, %al
	out %al, $0x40
halt:
	hlt
	jmp halt
.size __bootstg2, . - _start
