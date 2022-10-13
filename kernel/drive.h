#pragma once

#include "types.h"

typedef struct {
    
    uint8_t drive_num;
    uint8_t sectors_per_track;
    uint8_t heads;
    
} drive_t;

uint8_t init_drive(drive_t* drive, uint8_t drive_num);
