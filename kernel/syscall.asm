[bits 16]

section _TEXT class=CODE

extern _puts
extern return_from_program

global _int21
_int21:
    
    cmp ah, 00h
    je syscall_ah00
    
    cmp ah, 01h
    je syscall_ah01
    
    cmp ah, 09h
    je syscall_ah09
    
    iret
    
syscall_ah00:
    
    add sp, 6
    jmp return_from_program
    
syscall_ah01:
    
    push bx
    
    xor ah, ah
    int 16h
    
    mov ah, 0eh
    xor bx, bx
    int 10h
    
    pop bx
    
    cmp al, 3
    jne .skip_int23
    
    int 23h
    
.skip_int23:
    
    iret
    
syscall_ah09:
    
    push ax
    push cx
    push dx
    
    call _puts
    
    pop dx
    pop cx
    pop ax
    
    iret
    
global _int23
_int23:
    
    add sp, 12
    jmp return_from_program
    