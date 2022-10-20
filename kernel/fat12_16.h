#pragma once

#include "drive.h"

typedef struct fat12_16 {
    
    drive_t* drive;
    
    uint8_t sectors_per_cluster;
    uint16_t reserved_sectors;
    uint8_t fats;
    uint16_t root_dir_entries;
    uint16_t sectors_per_fat;
    
    uint32_t root_dir_start;
    uint32_t data_start;
    
} fat12_16_t;

typedef struct fat12_16_bpb {
    
    uint8_t jmp[3];
    uint8_t oem[8];
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint16_t reserved_sectors;
    uint8_t fats;
    uint16_t root_dir_entries;
    uint16_t total_sectors;
    uint8_t media;
    uint16_t sectors_per_fat;
    uint16_t sectors_per_track;
    uint16_t heads;
    uint32_t hidden_sectors;
    uint32_t large_sector_count;
    
    // fat 12/16 specific
    
    uint8_t drive;
    uint8_t nt_flags;
    uint8_t signature;
    uint32_t serial;
    uint8_t label[11];
    uint8_t sys_id[8];
    
    // boot code
    
    uint8_t boot_code[448];
    uint16_t boot_signature;
    
} fat12_16_bpb_t;
