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
