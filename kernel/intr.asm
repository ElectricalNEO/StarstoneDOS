[bits 16]

section _ENTRY class=CODE

global _int10h_teletype_output
_int10h_teletype_output:
    
    push bx
    push bp
    mov bp, sp
    
    ; bp - bp backup
    ; bp + 2 - bx backup
    ; bp + 4 - return address
    ; bp + 6 - character
    ; bp + 8 - page
    ; bp + 10 - foreground
    
    mov ah, 0eh
    mov al, [bp + 6]
    mov bh, [bp + 8]
    mov bl, [bp + 10]
    
    int 10h
    
    mov sp, bp
    pop bp
    pop bx
    ret
    
