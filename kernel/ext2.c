#include "ext2.h"
#include "fs.h"
#include "stdio.h"
#include "divmul32.h"
#include "heap.h"
#include "memory.h"

uint8_t init_ext2(partition_t* partition, fs_t* fs) {
    
    fs->partition = *partition;
    
    if(partition_read_sector(partition, 2, 0, (uint16_t)disk_tmp_buffer)) return 1;
    
    fs->fs.ext2.block_size = 1024 << ((ext2_superblock_t*)disk_tmp_buffer)->block_size;
    fs->fs.ext2.blocks_per_group = ((ext2_superblock_t*)disk_tmp_buffer)->blocks_per_group;
    fs->fs.ext2.inodes_per_group = ((ext2_superblock_t*)disk_tmp_buffer)->inodes_per_group;
    
    if(((ext2_superblock_t*)disk_tmp_buffer)->version_major) {
        
        fs->fs.ext2.inode_size = ((ext2_superblock_t*)disk_tmp_buffer)->inode_size;
        
    } else {
        
        fs->fs.ext2.inode_size = 128;
        
    }
    
    if(fs->fs.ext2.block_size == 1024) fs->fs.ext2.first_group = 2;
    else fs->fs.ext2.first_group = 1;
    
    fs->type = EXT2;
    
    return 0;
    
}

uint8_t ext2_read_inode(fs_t* fs, uint32_t inode_index, ext2_inode_t* inode) {
    
    uint32_t group;
    uint32_t index;
    uint32_t block;
    
    if(fs->type != EXT2) return 1;
    
    group = divide32(inode_index - 1, fs->fs.ext2.inodes_per_group);
    index = remainder32(inode_index - 1, fs->fs.ext2.inodes_per_group);
    block = divide32(multiply32(index, fs->fs.ext2.inode_size), fs->fs.ext2.block_size);
    
    if(partition_read_sector(&fs->partition, multiply32(fs->fs.ext2.first_group, fs->fs.ext2.block_size / 512) + divide32(multiply32(group, sizeof(ext2_block_group_descriptor_t)), 512), 0, (uint16_t)disk_tmp_buffer)) {
        
        return 1;
        
    }
    
    block += ((ext2_block_group_descriptor_t*)((uint32_t)disk_tmp_buffer + remainder32(multiply32(group, sizeof(ext2_block_group_descriptor_t)), 512)))->inode_table;
    
    if(partition_read_sector(&fs->partition, multiply32(block, fs->fs.ext2.block_size / 512) + divide32(index, 512 / fs->fs.ext2.inode_size), 0, (uint16_t)disk_tmp_buffer)) return 1;
    
    memcpy((void*)((uint32_t)disk_tmp_buffer + multiply32(remainder32(index, 512 / fs->fs.ext2.inode_size), fs->fs.ext2.inode_size)), inode, sizeof(ext2_inode_t));
    
    return 0;
    
}
