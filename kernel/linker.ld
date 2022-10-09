ENTRY(main);
OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386)
SECTIONS {
    
    . = 0x500;
    .text : AT(0x500) {
        *(.text);
    }
    
}