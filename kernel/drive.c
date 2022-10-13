#include "drive.h"

uint8_t get_drive_sectors_per_track(uint8_t drive_num);
#pragma aux get_drive_sectors_per_track = \
"mov ah, 8" \
"xor di, di" \
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
