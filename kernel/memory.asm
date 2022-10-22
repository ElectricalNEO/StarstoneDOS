[bits 16]

section _TEXT class=CODE

global _memcpy_seg
_memcpy_seg:
    
    push bp
    push ds
    push es
    push di
    push si
    mov bp, sp
    
    ; bp + 0 - si
    ; bp + 2 - di
    ; bp + 4 - es
    ; bp + 6 - ds
    ; bp + 8 - bp
    ; bp + 10 - return address
    ; bp + 12 - dest_seg
    ; bp + 14 - dest_off
    ; bp + 16 - src_seg
    ; bp + 18 - src_off
    ; bp + 20 - size
    
    mov ax, [bp + 16]
    mov ds, ax
    
    mov ax, [bp + 12]
    mov es, ax
    
    mov si, [bp + 18]
    mov di, [bp + 14]
    
    mov cx, [bp + 20]
    
    rep movsb
    
    mov sp, bp
    pop si
    pop di
    pop es
    pop ds
    pop bp
    
    ret