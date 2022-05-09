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
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

# The kernel entry point.
.section .text
.global _start
.type _start, @function
_start:
	# Clear other regs
	xor eax, eax
	xor ebx, ebx
	xor ecx, ecx
	mov ah, 0x0E # Write char
	mov bh, 0x00 # to screen
	mov bl, 0x0F # White
	mov cx, 0x0001 # Print once
	mov al, '1'
	int 0x10
	cli
1:	hlt
	jmp 1b

	mov stack_top, esp
	# Call the global constructors.
	call _init

	# Transfer control to the main kernel.
	call kernel_main

	# Hang if kernel_main unexpectedly returns.
	cli
2:	hlt
	jmp 2b
.size _start, . - _start
