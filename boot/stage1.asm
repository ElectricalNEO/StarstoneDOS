[bits 16]
[org 0x7c00]

jmp boot
nop

bpb:
.oem: db "12345678"
.bytes_per_sector: dw 0
.sectors_per_cluster: db 0
.reserved_sectors: dw 0
.fats: db 0
.root_dir_entries: dw 0
.total_sectors: dw 0
.media: db 0
.sectors_per_fat: dw 0
.sectors_per_track: dw 0
.heads: dw 0
.hidden_sectors: dd 0
.large_sector_count: dd 0
.drive: db 0
.nt_flags: db 0
.signature: db 0
.serial: dd 0
.volume_label: db "12345678901"
.system_id: db "12345678"

boot: jmp word 0:start
start:
	mov ah, 0xe
	mov al, 'A'
	xor bx, bx
	int 0x10
	jmp $

times 510 - ($ - $$) db 0
dw 0xaa55
