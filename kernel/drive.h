#pragma once

#include "types.h"

#pragma aux drive_read_sector_chs = \
"mov ah, 2" \
"mov al, 1" \
"int 13h" \
modify [ax] \
parm [dl] [cx] [dh] [es] [bx] \
value [ah]

typedef struct drive {
    
    uint8_t drive_num;
    uint8_t sectors_per_track;
    uint8_t heads;
    
} drive_t;

uint8_t init_drive(drive_t* drive, uint8_t drive_num);

uint8_t drive_read_sector_chs(uint8_t drive_num, uint16_t cylinder_sector, uint8_t head, uint16_t segment, uint16_t offset);
uint8_t drive_read_sector_lba(drive_t* drive, size_t lba, uint16_t segment, uint16_t offset);

uint8_t drive_read_sector_x(drive_t* drive, size_t lba, uint16_t segment, uint16_t offset, uint8_t retries);

void lba_to_chs(drive_t* drive, size_t lba, uint16_t* cylinder_sector, uint8_t* head);
