#pragma once

#include "types.h"
#include "list.h"

#pragma aux drive_read_sector_chs = \
"mov ah, 2" \
"mov al, 1" \
"int 13h" \
modify [ax] \
parm [dl] [cx] [dh] [es] [bx] \
value [ah]

#pragma aux check_drive = \
"mov ah, 8" \
"xor di, di" \
"mov es, di" \
"int 13h" \
modify [ah bl cx cx es di] \
parm [dl] \
value [ah]

typedef struct drive {
    
    uint8_t drive_num;
    uint8_t sectors_per_track;
    uint8_t heads;
    
} drive_t;

LINKED_LIST(drive_list_t, drive_t)

extern uint8_t disk_tmp_buffer[];

uint8_t check_drive(uint8_t drive_num);

uint8_t init_drive(drive_t* drive, uint8_t drive_num);

drive_list_t* init_drives();

uint8_t drive_read_sector_chs(uint8_t drive_num, uint16_t cylinder_sector, uint8_t head, uint16_t segment, uint16_t offset);
uint8_t drive_read_sector_lba(drive_t* drive, uint32_t lba, uint16_t segment, uint16_t offset);

uint8_t drive_read_sector_x(drive_t* drive, uint32_t lba, uint16_t segment, uint16_t offset, uint8_t retries);

void lba_to_chs(drive_t* drive, uint32_t lba, uint16_t* cylinder_sector, uint8_t* head);
