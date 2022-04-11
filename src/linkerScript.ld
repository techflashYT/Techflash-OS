ENTRY(startup)
OUTPUT_FORMAT(elf64-x86-64)
SECTIONS {
    . = 0x7c00;
    .text : { *(.boot) *(.text) }
    .rodata : { *(.rodata) }
    .data : { *(.data) }
    .bss : { *(.bss) }
}
