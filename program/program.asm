[bits 16]
[org 0x100]

mov ax, cs
mov ds, ax

mov ah, 09h
mov dx, hello_world
int 21h

mov ah, 01h
int 21h

xor ah, ah
int 21h

hello_world: db "Hello World! Press any key to continue...", 13, 10, '$'
