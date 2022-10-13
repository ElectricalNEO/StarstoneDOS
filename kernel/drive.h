#pragma once

#include "types.h"

typedef struct drive {
    
    uint8_t drive_num;
    uint8_t sectors_per_track;
    uint8_t heads;
    
} drive_t;

uint8_t init_drive(drive_t* drive, uint8_t drive_num);

void lba_to_chs(drive_t* drive, size_t lba, uint16_t* cylinder_sector, uint8_t* head);
