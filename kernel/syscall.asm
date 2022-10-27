[bits 16]

section _TEXT class=CODE

extern _puts
extern return_from_program

global _syscall
_syscall:
    
    cmp ah, 00h
    je syscall_ah00
    
    cmp ah, 09h
    je syscall_ah09
    
    iret
    
syscall_ah00:
    
    add sp, 6
    jmp return_from_program
    
syscall_ah09:
    
    push ax
    push cx
    push dx
    
    call _puts
    
    pop dx
    pop cx
    pop ax
    
    iret
    
