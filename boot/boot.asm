[bits 16]
[org 0x7c00]

jmp boot
nop

bpb:
.oem: db "12345678" ; unused by this bootloader
.bytes_per_sector: dw 0 ; unused by this bootloader
.sectors_per_cluster: db 0
.reserved_sectors: dw 0
.fats: db 0
.root_dir_entries: dw 0
.total_sectors: dw 0 ; unused by this bootloader
.media: db 0 ; unused by this bootloader
.sectors_per_fat: dw 0
.sectors_per_track: dw 0
.heads: dw 0
.hidden_sectors: dd 0 ; unused by this bootloader
.large_sector_count: dd 0 ; unused by this bootloader
.drive: db 0
.nt_flags: db 0 ; unused by this bootloader
.signature: db 0 ; unused by this bootloader
.serial: dd 0 ; unused by this bootloader
.volume_label: db "12345678901" ; unused by this bootloader
.system_id: db "12345678" ; unused by this bootloader

; UNUSED BPB ENTRIES CAN BE USED FOR UNINITIALIZED VARIABLES

root_dir_start equ bpb.oem ; 2 bytes
data_start equ (bpb.oem + 2) ; 2 bytes

boot: jmp word 0:start
start:
	
	; SET SEGMENT REGISTERS AND STACK POINTER
	
	xor ax, ax
	mov ds, ax
	mov es, ax
	
	mov ax, 0xffff
	mov ss, ax
	mov sp, 0
	
	; SAVE DRIVE NUMBER
	
	mov [bpb.drive], dl
	
	; GET DRIVE PARAMETERS
	
	mov ah, 8
	xor di, di
	int 13h
	
	inc dh
	mov [bpb.heads], dh
	
	and cl, 0x3f
	mov [bpb.sectors_per_track], cl
	
	xor bx, bx
	mov es, bx
	
	; DISPLAY BOOT MESSAGE
	
	mov si, data.booting_up
	call print
	
	; FIND THE FILE IN THE ROOT DIRECTORY
	
	xor ah, ah
	mov al, [bpb.fats]
	mul word [bpb.sectors_per_fat]
	add ax, [bpb.reserved_sectors] ; ax = root directory start
	mov [root_dir_start], ax
	
	xor cx, cx
.find_file:
	
	push cx
	
	mov ax, cx
	xor dx, dx
	mov bx, 16
	div bx
	
	cmp dx, 0
	jne .skip_load_rd
	
	push dx
	
	add ax, [root_dir_start]
	call lba_to_chs
	mov al, 1
	mov bx, 0x7e00
	mov di, 5
	call disk_read
	cmp ah, 0
	mov dx, 0
	jne .error
	
	pop dx
	
.skip_load_rd:
	
	mov ax, dx
	
	mov si, data.filename
	mov bx, 32
	mul bx
	add ax, 0x7e00
	mov di, ax
	push di
	mov cx, 11
	rep cmpsb
	pop di
	mov cx, [di + 26] ; cluster
	je .found
	
	pop cx
	
	cmp cx, [bpb.root_dir_entries]
	mov dx, 1
	je .error
	inc cx
	jmp .find_file
	
.found: ; cx = cluster
	
	mov ax, [bpb.root_dir_entries]
	mov bx, 32
	mul bx
	add ax, 511
	xor dx, dx
	mov bx, 512
	div bx
	add ax, [root_dir_start]
	mov [data_start], ax
	
	mov si, 0x500
	
.read_file:
	
	push cx
	
	mov ax, cx
	sub ax, 2
	xor bh, bh
	mov bl, [bpb.sectors_per_cluster]
	mul bx
	add ax, [data_start]
	call lba_to_chs
	mov al, [bpb.sectors_per_cluster]
	mov bx, si
	mov di, 5
	call disk_read
	
	cmp ah, 0
	mov dx, 2
	jne .error
	
	xor ah, ah
	mov al, [bpb.sectors_per_cluster]
	mov bx, 512
	mul bx
	add si, bx
	
	pop cx
	
	mov ax, cx
	mov bx, 3
	mul bx
	mov bx, 2
	xor dx, dx
	div bx
	
	xor dx, dx
	mov bx, 512
	div bx
	add ax, [bpb.reserved_sectors]
	push dx
	
	push cx
	call lba_to_chs
	mov al, 1
	mov bx, 0x7e00
	mov di, 5
	call disk_read
	
	cmp ah, 0
	mov dx, 3
	jne .error
	
	pop cx
	pop bx
	add bx, 0x7e00
	
	mov ax, [bx]
	
	and cx, 1
	jnz .odd
.even:
	and ax, 0xfff
	jmp .skip_odd
	
.odd:
	shr ax, 4
	
.skip_odd:
	mov cx, ax
	cmp cx, 0xff7
	jb .read_file
	
	xor ah, ah
	mov al, [bpb.drive]
	push ax
	
	jmp 0:0x500
	
.error:
	mov si, data.error
	call print
	jmp $

; INPUT ds:si - string
print:
	push ax
	push bx
	pushf
	
	mov ah, 0xe
	xor bx, bx
.loop:
	lodsb
	or al, al
	jz .end
	int 0x10
	jmp .loop
.end:
	popf
	pop bx
	pop ax
	
	ret

; INPUT ax - lba
; OUTPUT cx [6-15] - track
; OUTPUT cx [0-5] - sector
; OUTPUT dh - head
; DESTROYED dl
; DESTROYED ax
lba_to_chs:
	
	xor dx, dx
	div word [bpb.sectors_per_track]
	
	mov cx, dx
	
	xor dx, dx
	div word [bpb.heads]
	
	; cx = sector - 1
	; dx = head
	; ax = track
	
	inc cx
	
	mov ch, al
	shl ah, 6
	or cl, ah
	
	mov dh, dl
	
	ret

; INPUT cx [6-15] - track
; INPUT cx [0-5] - sector
; INPUT dh - head
; INPUT al - sector count
; INPUT es:bx - memory
; INPUT di - retries
; OUTPUT ah - status
; OUTPUT al - sectors read count
; OUTPUT dl - drive
disk_read:
	
.loop:
	
	cmp di, 0
	jz .end
	
	mov ah, 2
	mov dl, [bpb.drive]
	int 13h
	jnc .end
	
	dec di
	jmp .loop
	
.end:
	ret

data:
.booting_up: db "Booting up Starstone...", 13, 10, 0
.filename: db "STARKRNLSYS"
.error: db "ERROR!", 13, 10, 0

times 510 - ($ - $$) db 0
dw 0xaa55
