#pragma once

#include "types.h"
#include "drive.h"

typedef struct mbr_partition_entry {
    
    uint8_t bootable;
    uint8_t start_head;
    uint16_t start_cylinder_sector;
    uint8_t system_id;
    uint8_t end_head;
    uint16_t end_cylinder_sector;
    uint32_t lba_start;
    uint32_t total_sectors;
    
} mbr_partition_entry_t;

typedef struct mbr {
    
    uint8_t boot_code[440];
    uint32_t disk_id;
    uint16_t reserved;
    mbr_partition_entry_t partitions[4];
    uint16_t boot_signature;
    
} mbr_t;

typedef struct partition {
    
    drive_t drive;
    uint32_t lba;
    uint8_t part_num; // 0 - 3
    uint8_t bootable;
    
} partition_t;

LINKED_LIST(partition_list_t, partition_t)

partition_list_t* init_partitions(drive_list_t* drives);

uint8_t partition_read_sector(partition_t* partition, uint32_t lba, uint16_t segment, uint16_t offset);
