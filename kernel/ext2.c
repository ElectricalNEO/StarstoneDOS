#include "ext2.h"
#include "fs.h"
#include "stdio.h"
#include "divmul32.h"
#include "heap.h"
#include "memory.h"
#include "string.h"

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

uint8_t ext2_read_file_sector(fs_t* fs, ext2_inode_t* inode, uint32_t sector_i, uint16_t segment, uint16_t offset) {
    
    uint32_t block;
    uint32_t sector;
    
    if(fs->type != EXT2) return 1;
    
    block = divide32(sector_i, divide32(fs->fs.ext2.block_size, 512));
    sector = remainder32(sector_i, divide32(fs->fs.ext2.block_size, 512));\
    
    if(block < 12) { // direct
        
        if(!inode->direct_block_pointer[block]) return 1;
        sector += multiply32(inode->direct_block_pointer[block], divide32(fs->fs.ext2.block_size, 512));
        
    } else if(block < 128 + 12) { // singly indirect
        
        uint32_t s;
        uint16_t r;
        
        if(!inode->singly_indirect_block_pointer) return 1;
        
        s = divide32(inode->singly_indirect_block_pointer + multiply32(block - 12, 4), 512);
        r = remainder32(inode->singly_indirect_block_pointer + multiply32(block - 12, 4), 512);
        
        if(partition_read_sector(&fs->partition, s, 0, (uint16_t)disk_tmp_buffer)) return 1;
        
        if(!*(uint32_t*)((uint16_t)disk_tmp_buffer + r)) return 1;
        
        sector += multiply32(*(uint32_t*)((uint16_t)disk_tmp_buffer + r), divide32(fs->fs.ext2.block_size, 512));
        
    } else { // doubly indirect
        
        uint32_t s;
        uint16_t r;
        
        if(!inode->doubly_indirect_block_pointer) return 1;
        
        s = divide32(inode->doubly_indirect_block_pointer + multiply32(block - 12 - 128, 4), 512);
        r = divide32(inode->doubly_indirect_block_pointer + multiply32(block - 12 - 128, 4), 512);
        
        if(partition_read_sector(&fs->partition, s, 0, (uint16_t)disk_tmp_buffer)) return 1;
        
        if(!*(uint32_t*)((uint16_t)disk_tmp_buffer + r)) return 1;
        
        s = divide32(multiply32(*(uint32_t*)((uint16_t)disk_tmp_buffer + r), divide32(fs->fs.ext2.block_size, 512)) + multiply32(block - 12 - 128, 4), 512);
        r = remainder32(multiply32(*(uint32_t*)((uint16_t)disk_tmp_buffer + r), divide32(fs->fs.ext2.block_size, 512)) + multiply32(block - 12 - 128, 4), 512);
        
        if(partition_read_sector(&fs->partition, s, 0, (uint16_t)disk_tmp_buffer)) return 1;
        
        if(!*(uint32_t*)((uint16_t)disk_tmp_buffer + r)) return 1;
        
        sector += multiply32(*(uint32_t*)((uint16_t)disk_tmp_buffer + r), divide32(fs->fs.ext2.block_size, 512));
        
    }
    
    if(partition_read_sector(&fs->partition, sector, segment, offset)) return 1;
    
    return 0;
    
}

uint8_t ext2_read_file_bytes(fs_t* fs, ext2_inode_t* file, uint16_t seek, uint16_t size, uint16_t segment, uint16_t offset) {
    
    size_t start = seek / 512;
    size_t start_off = seek % 512;
    size_t end_size = (seek + size) % 512;
    size_t end = (seek + size) / 512;
    
    size_t buf_off = 0;
    size_t i;
    size_t off;
    size_t _size;
    
    if(fs->type != EXT2) return 1;
    
    for(i = start; i <= end; i++) {
        
        if(ext2_read_file_sector(fs, file, i, 0, (size_t)disk_tmp_buffer)) return 1;
        
        off = 0;
        _size = 512;
        
        if(i == end) {
            
            _size = end_size;
            
        }
        
        if(i == start) {
            
            off = start_off;
            _size -= start_off;
            
        }
        
        memcpy_seg(segment, offset + buf_off, 0, (size_t)disk_tmp_buffer + off, _size);
        buf_off += _size;
        
    }
    
    return 0;
    
}

uint8_t ext2_find_file(fs_t* fs, ext2_inode_t* directory, ext2_inode_t* output, char* name) {
    
    uint32_t dir_entry_offset = 0;
    uint8_t name_len;
    uint16_t size;
    char* name_d;
    
    if(fs->type != EXT2) return 1;
    
    while(1) {
        
        if(ext2_read_file_bytes(fs, directory, dir_entry_offset + 6, 1, 0, (uint16_t)&name_len)) return 1;
        
        if(name_len != strlen(name)) {
            
            if(ext2_read_file_bytes(fs, directory, dir_entry_offset + 4, 2, 0, (uint16_t)&size)) return 1;
            dir_entry_offset += size;
            continue;
            
        }
        
        name_d = kmalloc(name_len);
        
        if(ext2_read_file_bytes(fs, directory, dir_entry_offset + 8, name_len, 0, (uint16_t)name_d)) {
            
            kfree(name_d);
            return 1;
            
        }
        
        if(!memcmp(name_d, name, name_len)) {
            
            uint32_t inode;
            kfree(name_d);
            
            if(ext2_read_file_bytes(fs, directory, dir_entry_offset, 4, 0, (uint16_t)&inode)) {
                
                return 1;
                
            }
            
            return ext2_read_inode(fs, inode, output);
            
        } else {
            
            if(ext2_read_file_bytes(fs, directory, dir_entry_offset + 4, 2, 0, (uint16_t)&size)) return 1;
            dir_entry_offset += size;
            
        }
        
        kfree(name_d);
        
    }
    
    return 0;
    
}
