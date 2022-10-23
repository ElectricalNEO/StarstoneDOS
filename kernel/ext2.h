#pragma once

#define EXT2_CLEAN 1
#define EXT2_ERROR 2

#define EXT2_ERROR_IGNORE 1
#define EXT2_ERROR_REMOUNT_RO 2
#define EXT2_ERROR_PANIC 3

#define EXT2_SIGNATURE 0xef53

#include "types.h"
#include "mbr.h" 

typedef struct ext2_superblock_t {
    
    uint32_t inodes;
    uint32_t blocks;
    uint32_t superuser_blocks;
    uint32_t unallocated_blocks;
    uint32_t unallocated_inodes;
    uint32_t superblock_block;
    uint32_t block_size;
    uint32_t fragment_size;
    uint32_t blocks_per_group;
    uint32_t fragments_per_group;
    uint32_t inodes_per_group;
    uint32_t last_mount_time;
    uint32_t last_write_time;
    uint16_t mounts_since_check;
    uint16_t mounts_before_check;
    uint16_t signature;
    uint16_t fs_state;
    uint16_t error_action;
    uint16_t version_minor;
    uint32_t last_check_time;
    uint32_t interval_between_checks;
    uint32_t os;
    uint32_t reserved_block_user;
    uint32_t reserved_block_group;
    
} ext2_superblock_t;

typedef struct ext2_t {
    
    uint8_t test;
    
} ext2_t;

typedef struct fs_t fs_t;

uint8_t init_ext2(partition_t* partition, fs_t* fs);
