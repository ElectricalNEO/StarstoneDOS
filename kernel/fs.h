#pragma once

#include "mbr.h"

#include "fat12_16.h"
#include "ext2.h"

typedef enum filesystem_type {
    
    FAT12,
    FAT16,
    EXT2,
    UNKNOWN
    
} filesystem_type_t;

typedef union fs_union_t {
    
    fat12_16_t fat12_16;
    ext2_t ext2;
    
} fs_union_t;

typedef struct fs_t {
    
    filesystem_type_t type;
    partition_t partition;
    fs_union_t fs;
    
} fs_t;

LINKED_LIST(fs_list_t, fs_t)

filesystem_type_t detect_filesystem(partition_t* partition);

uint8_t init_fs(fs_t* fs, partition_t* partition);
