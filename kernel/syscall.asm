[bits 16]

section _TEXT class=CODE

extern _puts
extern _putc
extern return_from_program

global _int21
_int21:
    
    cmp ah, 00h
    je syscall_ah00 ; TERMINATE PROGRAM
    
    cmp ah, 01h
    je syscall_ah01 ; READ CHARACTER FROM STDIN WITH ECHO
    
    cmp ah, 02h
    je syscall_ah02 ; WRITE CHARACTER TO STDOUT
    
    cmp ah, 06h
    je syscall_ah06 ; DIRECT CONSOLE OUTPUT
    
    cmp ah, 09h
    je syscall_ah09 ; WRITE STRING TO STDOUT
    
    iret
    
syscall_ah00: ; TERMINATE PROGRAM
    
    add sp, 6
    jmp return_from_program
    
syscall_ah01: ; READ CHARACTER FROM STDIN WITH ECHO
    
    push bx
    
    xor ah, ah
    int 16h
    
    push dx
    push cx
    push ax
    
    call _putc
    
    pop ax
    
    cmp al, 0dh
    jne .skip_print_nl
    
    push ax
    push 0ah
    call _putc
    add sp, 2
    pop ax
    
.skip_print_nl:
    
    pop cx
    pop dx
    
    pop bx
    
    cmp al, 3
    jne .skip_int23
    
    int 23h
.skip_int23:
    
    iret

syscall_ah02: ; WRITE CHARACTER TO STDOUT
    
    push bx
    
    push ax
    push cx
    push dx
    
    call _putc
    
    pop dx
    
    pop cx
    pop ax
    
    pop bx
    
    cmp dl, 3
    jne .skip_int23
    
    int 23h
.skip_int23:
    iret
    
syscall_ah06: ; DIRECT CONSOLE OUTPUT
    
    push bx
    
    push ax
    push cx
    push dx
    
    call _putc
    
    pop dx
    pop cx
    pop ax
    
    pop bx
    
    iret
    
syscall_ah09: ; WRITE STRING TO STDOUT
    
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
    