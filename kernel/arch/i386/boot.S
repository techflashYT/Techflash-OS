# Declare constants for the multiboot header.
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

# Declare a header as in the Multiboot Standard.
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# Reserve a stack for the initial thread.
.section .bss
.align 16
stackBottom:
.skip 16384 # 16 KiB
stackTop:

# The kernel entry point.
.section .text
.global _start
.global _disableCursor
.type _disableCursor, @function
.type _start, @function
_start:
	movl $stackTop, %esp
	movl 16(%ebx), %ecx

	# Call the global constructors.
	call _init
	# Check if A20 Gate is already enabled from grub 
	pushal
	mov $0x112345, %edi  # odd megabyte address.
	mov $0x012345, %esi  # even megabyte address.
	mov %esi, (%esi)     # making sure that both addresses contain diffrent values.
	mov %esi, (%esi)     # (if A20 line is cleared the two pointers would point to the address 0x012345 that would contain 0x112345 (edi)) 
	cmpsd             # compare addresses to see if the're equivalent.
	popal
	jne __bootstg2 # if A20 is on, jmp to call boot stage2.

	# Enable A20 Gate if it's not enabled.
	in $0x92, %al
	or $2, %al
	out %al, $0x92

_disableCursor:
	pushf
	push %eax
	push %edx

	mov $0x3d4, %dx
	mov $0xa, %al
	out %al, %dx

	inc %dx
	mov $0x20, %al
	out %al, %dx

	pop %edx
	pop %eax
	popf
	ret

__bootstg2:
	call _disableCursor
	# Just in case interrupts are enabled, disable them.
	cli
	# Also just in case, clear all the regs
	xor %eax, %eax
	xor %ebx, %ebx
	xor %edx, %edx
	xor %esi, %esi
	xor %edi, %edi
	xor %ebp, %ebp
	push %ecx # Push the grub args to the stack before zeroing ecx
	xor %ecx, %ecx
	# Transfer control to the main kernel.
	call kernelMain

	# Hang if kernelMain unexpectedly returns.
	cli
1:	hlt
	jmp 1b
.size __bootstg2, . - _start
