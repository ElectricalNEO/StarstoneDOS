[bits 16]
[org 0x7c00]

jmp 0:boot
boot:
	mov ah, 0xe
	mov al, 'A'
	xor bx, bx
	int 0x10
	jmp $

times 510 - ($ - $$) db 0
dw 0xaa55
