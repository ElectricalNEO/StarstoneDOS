[bits 16]

section _TEXT class=CODE

global _jump_to_program
global return_from_program

_jump_to_program:
    
    pusha
    push es
    
    jmp word 0x1000:0x100
    
return_from_program:
    
    xor ax, ax
    mov ds, ax
    mov ss, ax
    pop es
    popa
    
    ret
