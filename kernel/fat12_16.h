#pragma once

#include "mbr.h"

#define FAT12 0
#define FAT16 1

typedef struct fat12_16 {
    
    partition_t partition;
    
    uint8_t sectors_per_cluster;
    uint16_t reserved_sectors;
    uint8_t fats;
    uint16_t root_dir_entries;
    uint16_t sectors_per_fat;
    
    uint32_t root_dir_start;
    uint32_t root_dir_sectors;
    uint32_t data_start;
    
    uint8_t type; // FAT12 or FAT16
    
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

typedef struct fat_dir_entry {
    
    uint8_t filename[11];
    uint8_t attributes;
    uint8_t nt_reserved;
    uint8_t creation_time_tenths_seconds;
    uint16_t creation_time;
    uint16_t creation_date;
    uint16_t last_access_date;
    uint16_t cluster_high;
    uint16_t last_modification_time;
    uint16_t last_modification_date;
    uint16_t cluster_low;
    uint32_t size;
    
} fat_dir_entry_t;

uint8_t init_fat12_16(partition_t* partition, fat12_16_t* fs);

fat_dir_entry_t fat12_16_find_in_root_dir(fat12_16_t* fs, char* filename);

uint8_t fat12_16_read_file_sector(fat12_16_t* fs, fat_dir_entry_t* file, uint16_t sector, uint16_t segment, uint16_t offset);
uint8_t fat12_16_read_file(fat12_16_t* fs, fat_dir_entry_t* file, uint16_t segment, uint16_t offset);
uint8_t fat12_16_read_file_bytes(fat12_16_t* fs, fat_dir_entry_t* file, uint16_t seek, uint16_t size, uint16_t segment, uint16_t offset);
