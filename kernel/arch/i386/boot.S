# Constants for multiboot 2 Header
.set MAGIC,    0xE85250D6    # magic number
.set ARCH,     0x00000000    # 32-bit x86
.set HEAD_LEN, 0x00000010    # Header length
.set CHECKSUM, -(HEAD_LEN + ARCH + MAGIC) # checksum, must be a uint32 that, when the above things are added to it, equals 0.  So all we do is do the exact same thing, but make it negative.  Like how -1 + 1 = 0, the same principle applies here.

# Multiboot 2 header
.section .multiboot
.long MAGIC
.long ARCH
.long HEAD_LEN
.long CHECKSUM
# ###### FLAGS FOR REAL 640x480 FRAMEBUFFER #####
# .long 0
# .short 5 # type = 5
# .short 0 # flags = 0
# .long 20 # size = 20
# .long 640 # width
# .long 480 # height
# .long 8 # depth
# .long 0 # END
# But rather than that, lets just use basic console because it's so much easier to use
.word 4
.word 0
.long 12
.long 0x03
.balign 8
# Request some info
.word 1
.word 0
.long 4*6+8
.long 5 # BIOS boot device
.long 1 # command line
.long 3 # modules
.long 9 # ELF symbols
.long 6 # memory map
.long 10 # APM table
.balign 8
.long 0
.long 0
.long 0
# Reserve a stack for the initial thread.
.section .bss
.align 16
stackBottom:
.skip 131072 # 128K of stack
stackTop:

# The kernel entry point.
.section .text
.global _start
.global _disableCursor
.type _disableCursor, @function
.type _start, @function

_start:
	movl $stackTop, %esp
	pushl %ebx
	pushl %eax

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
	xor %esi, %esi
	xor %edi, %edi
	xor %ebp, %ebp
	xor %ecx, %ecx
	xor %edx, %edx
	# Transfer control to the main kernel.
	call kernelMain

	# Hang if kernelMain unexpectedly returns.
	cli
1:	hlt
	jmp 1b
.size __bootstg2, . - _start
