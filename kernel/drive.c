#include "drive.h"
#include "divmul32.h"
#include "heap.h"

uint8_t get_drive_sectors_per_track(uint8_t drive_num);
#pragma aux get_drive_sectors_per_track = \
"mov ah, 8" \
"xor di, di" \
"mov es, di" \
"int 13h" \
"jc error" \
"and cl, 0x3f" \
"jmp end" \
"error:" \
"mov cl, 0" \
"end:" \
modify [ax bl cx dx es di] \
parm [dl] \
value [cl]

uint8_t get_drive_heads(uint8_t drive_num);
#pragma aux get_drive_heads = \
"mov ah, 8" \
"xor di, di" \
"mov es, di" \
"int 13h" \
"jc error" \
"inc dh" \
"jmp end" \
"error:" \
"mov dh, 0" \
"end:" \
modify [ax bl cx dx es di] \
parm [dl] \
value [dh]

uint8_t init_drive(drive_t* drive, uint8_t drive_num) {
    
    drive->drive_num = drive_num;
    
    drive->sectors_per_track = get_drive_sectors_per_track(drive_num);
    if(!drive->sectors_per_track) return 1;
    
    drive->heads = get_drive_heads(drive_num);
    if(!drive->heads) return 1;
    
    return 0;
    
}

drive_t* init_drives(uint8_t* connected) {
    
    uint8_t i;
    drive_t* drives;
    uint8_t drive = 0;
    *connected = 0;
    
    for(i = 0; i < 0xff; i++) if(!check_drive(i)) (*connected)++;
    
    drives = kmalloc(*connected * sizeof(drive_t));
    
    for(i = 0; i < 0xff; i++) {
        
        if(check_drive(i)) continue;
        
        init_drive(&drives[drive], i);
        drive++;
        
    }
    
    return drives;
    
}

uint8_t drive_read_sector_lba(drive_t* drive, uint32_t lba, uint16_t segment, uint16_t offset) {
    
    uint16_t cylinder_sector;
    uint8_t head;
    
    lba_to_chs(drive, lba, &cylinder_sector, &head);
    
    return drive_read_sector_chs(drive->drive_num, cylinder_sector, head, segment, offset);
    
}

uint8_t drive_read_sector_x(drive_t* drive, uint32_t lba, uint16_t segment, uint16_t offset, uint8_t retries) {
    
    uint8_t status = 1;
    uint16_t cylinder_sector;
    uint8_t head;
    uint8_t i;
    
    lba_to_chs(drive, lba, &cylinder_sector, &head);
    
    for(i = 0; i < retries && status; i++) {
        
        status = drive_read_sector_chs(drive->drive_num, cylinder_sector, head, segment, offset);
        
    }
    
    return status;
    
}

void lba_to_chs(drive_t* drive, uint32_t lba, uint16_t* cylinder_sector, uint8_t* head) {
    
    uint16_t cylinder = divide32(divide32(lba, drive->sectors_per_track), drive->heads);
    uint8_t sector = remainder32(lba, drive->sectors_per_track) + 1;
    
    *head = remainder32(divide32(lba, drive->sectors_per_track), drive->heads);
    *cylinder_sector = ((cylinder & 0xff) << 8) | ((cylinder & 0x300) >> 2 ) | sector;
    
}
