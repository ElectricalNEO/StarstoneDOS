#pragma once

#define EXT2_CLEAN 1
#define EXT2_ERROR 2

#define EXT2_ERROR_IGNORE 1
#define EXT2_ERROR_REMOUNT_RO 2
#define EXT2_ERROR_PANIC 3

#define EXT2_SIGNATURE 0xef53

#define EXT2_OPTIONAL_PREALLOCATE_BLOCKS 0x01
#define EXT2_OPTIONAL_AFS_SERVER_INODES 0x02
#define EXT2_OPTIONAL_JOURNAL 0x04
#define EXT2_OPTIONAL_EXTENDED_ATTRIBUTE 0x08
#define EXT2_OPTIONAL_RESIZE 0x10
#define EXT2_OPTIONAL_HASH_INDEX 0x20

#define EXT2_REQUIRED_COMPRESSION 0x01
#define EXT2_REQUIRED_DIRECTORY_TYPE 0x02
#define EXT2_REQUIRED_JOURNAL_REPLAY 0x04
#define EXT2_REQUIRED_JOURNAL_DEVICE 0x08

#define EXT2_READ_ONLY_SPARSE_SUPERBLOCK 0x01
#define EXT2_READ_ONLY_64_BIT_FILE_SIZE 0x02
#define EXT2_READ_ONLY_BINARY_TREE 0x04

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
    uint32_t version_major;
    uint16_t reserved_block_user;
    uint16_t reserved_block_group;
    
    // extended superblock
    
    uint32_t first_nonreserved_inode;
    uint16_t inode_size;
    uint16_t superblock_group;
    uint32_t optional_features;
    uint32_t required_features;
    uint32_t ro_features;
    uint8_t id[16];
    uint8_t volume_name[16];
    uint8_t last_mount_path[64];
    uint32_t compression;
    uint8_t preallocate_blocks_files;
    uint8_t preallocate_blocks_directories;
    uint16_t unused;
    uint8_t journal_id[16];
    uint32_t journal_inode;
    uint32_t journal_device;
    uint32_t orphan_inode_list;
    
} ext2_superblock_t;

typedef struct ext2_block_group_descriptor_t {
    
    uint32_t block_usage_bitmap;
    uint32_t inode_usage_bitmap;
    uint32_t inode_table;
    uint16_t unallocated_blocks;
    uint16_t unallocated_inodes;
    uint16_t directories;
    uint8_t unused[14];
    
} ext2_block_group_descriptor_t;

typedef struct ext2_inode_t {
    
    uint16_t type_permissions;
    uint16_t user_id;
    uint32_t size_lower;
    uint32_t access_time;
    uint32_t creation_time;
    uint32_t modification_time;
    uint32_t deletion_time;
    uint16_t group_id;
    uint16_t hard_links;
    uint32_t sectors;
    uint32_t flags;
    uint32_t os_specific1;
    uint32_t direct_block_pointer[12];
    uint32_t singly_indirect_block_pointer;
    uint32_t doubly_indirect_block_pointer;
    uint32_t triply_indirect_block_pointer;
    uint32_t generation_number;
    uint32_t extended_attribute_block;
    uint32_t size_upper;
    uint32_t fragment;
    uint8_t os_specific2[12];
    
} ext2_inode_t;

typedef struct ext2_t {
    
    uint16_t block_size;
    uint32_t blocks_per_group;
    uint32_t inodes_per_group;
    uint32_t first_group;
    uint16_t inode_size;
    
} ext2_t;

typedef struct fs_t fs_t;

uint8_t init_ext2(partition_t* partition, fs_t* fs);
uint8_t ext2_read_inode(fs_t* fs, uint32_t inode_index, ext2_inode_t* inode);
