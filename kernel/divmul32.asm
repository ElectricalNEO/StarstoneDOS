[bits 16]

section _CODE class=CODE

global _multiply32
_multiply32:
    
    push bp
    mov bp, sp
    
    ; [bp] - bp backup
    ; [bp + 2] - return address
    ; [bp + 4] - a
    ; [bp + 8] - b
    
    mov eax, [bp + 4]
    mov ecx, [bp + 8]
    mul ecx
    
    mov sp, bp
    pop bp
    ret

global _divide32
_divide32:
    
    push bp
    mov bp, sp
    
    ; [bp] - bp backup
    ; [bp + 2] - return address
    ; [bp + 4] - a
    ; [bp + 8] - b
    
    mov eax, [bp + 4]
    mov ecx, [bp + 8]
    xor edx, edx
    div ecx
    
    mov ecx, eax
    shr eax, 16
    mov edx, eax
    mov eax, ecx
    
    mov sp, bp
    pop bp
    ret

global _remainder32
_remainder32:
    
    push bp
    mov bp, sp
    
    ; [bp] - bp backup
    ; [bp + 2] - return address
    ; [bp + 4] - a
    ; [bp + 8] - b
    
    mov eax, [bp + 4]
    mov ecx, [bp + 8]
    xor edx, edx
    div ecx
    
    mov eax, edx
    mov ecx, eax
    shr eax, 16
    mov edx, eax
    mov eax, ecx
    
    mov sp, bp
    pop bp
    ret
