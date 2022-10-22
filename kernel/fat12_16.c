#include "fat12_16.h"
#include "divmul32.h"
#include "memory.h"

uint8_t init_fat12_16(partition_t* partition, fat12_16_t* fs) {
    
    if(partition_read_sector(partition, 0, 0, (uint16_t)disk_tmp_buffer)) return 1;
    
    if(((fat12_16_bpb_t*)disk_tmp_buffer)->signature == 0x28 || ((fat12_16_bpb_t*)disk_tmp_buffer)->signature == 0x29) {
        
        uint32_t data_sectors;
        uint16_t total_clusters;
        uint32_t total_sectors = ((fat12_16_bpb_t*)disk_tmp_buffer)->total_sectors == 0 ? ((fat12_16_bpb_t*)disk_tmp_buffer)->large_sector_count : ((fat12_16_bpb_t*)disk_tmp_buffer)->total_sectors;
        fs->root_dir_sectors = ((((fat12_16_bpb_t*)disk_tmp_buffer)->root_dir_entries * 32) + 511) / 512;
        data_sectors = total_sectors - (((fat12_16_bpb_t*)disk_tmp_buffer)->reserved_sectors + (((fat12_16_bpb_t*)disk_tmp_buffer)->fats * ((fat12_16_bpb_t*)disk_tmp_buffer)->sectors_per_fat) + fs->root_dir_sectors);
        total_clusters = divide32(data_sectors, ((fat12_16_bpb_t*)disk_tmp_buffer)->sectors_per_cluster);
        
        if(total_clusters < 4085) fs->type = TYPE_FAT12;
        else fs->type = TYPE_FAT16;
        
        fs->data_start = ((fat12_16_bpb_t*)disk_tmp_buffer)->reserved_sectors + (((fat12_16_bpb_t*)disk_tmp_buffer)->fats * ((fat12_16_bpb_t*)disk_tmp_buffer)->sectors_per_fat) + fs->root_dir_sectors;
        fs->fats = ((fat12_16_bpb_t*)disk_tmp_buffer)->fats;
        fs->partition = *partition;
        fs->reserved_sectors = ((fat12_16_bpb_t*)disk_tmp_buffer)->reserved_sectors;
        fs->root_dir_entries = ((fat12_16_bpb_t*)disk_tmp_buffer)->root_dir_entries;
        fs->root_dir_start = ((fat12_16_bpb_t*)disk_tmp_buffer)->reserved_sectors + (((fat12_16_bpb_t*)disk_tmp_buffer)->fats * ((fat12_16_bpb_t*)disk_tmp_buffer)->sectors_per_fat);
        fs->sectors_per_cluster = ((fat12_16_bpb_t*)disk_tmp_buffer)->sectors_per_cluster;
        fs->sectors_per_fat = ((fat12_16_bpb_t*)disk_tmp_buffer)->sectors_per_fat;
        
        return 0;
        
    }
    
    return 1;
    
}

fat_dir_entry_t fat12_16_find_in_root_dir(fat12_16_t* fs, char* filename) {
    
    uint16_t root_dir_entry = 0;
    fat_dir_entry_t* dir = (fat_dir_entry_t*)disk_tmp_buffer;
    fat_dir_entry_t no_ret = {.cluster_low = 0};
    
    for(; root_dir_entry < fs->root_dir_entries; root_dir_entry++) {
        
        if(root_dir_entry % 16 == 0) {
            
            if(partition_read_sector(&fs->partition, fs->root_dir_start + root_dir_entry / 16, 0, (uint16_t)disk_tmp_buffer)) {
                
                return no_ret;
                
            }
            
        }
        
        if(memcmp(dir[root_dir_entry % 16].filename, filename, 11) == 0) {
            
            return dir[root_dir_entry % 16];
            
        }
        
    }
    
    return no_ret;
    
}

fat_dir_entry_t fat12_16_find_in_dir(fat12_16_t* fs, char* filename, fat_dir_entry_t* dir) {
    
    fat_dir_entry_t* dir_mem = (fat_dir_entry_t*)disk_tmp_buffer;
    uint16_t i = 0;
    fat_dir_entry_t no_ret = {.cluster_low = 0};
    
    while(!fat12_16_read_file_sector(fs, dir, i, 0, (uint16_t)disk_tmp_buffer)) {
        
        uint8_t entry = 0;
        
        for(; entry < 16; entry++) {
            
            if(memcmp(dir_mem[entry].filename, filename, 11) == 0) {
                
                return dir_mem[entry];
                
            }
            
        }
        
        i++;
        
    }
    
    return no_ret;
    
}

uint8_t fat12_16_read_file_sector(fat12_16_t* fs, fat_dir_entry_t* file, uint16_t sector, uint16_t segment, uint16_t offset) {
    
    uint16_t cluster_index = sector / fs->sectors_per_cluster;
    uint16_t sector_off = sector % fs->sectors_per_cluster;
    uint16_t cluster = file->cluster_low;
    uint8_t cluster_i = 0;
    
    while(1) {
        
        uint16_t clus_sector = ((cluster - 2) * fs->sectors_per_cluster) + fs->data_start;
        
        if(cluster_i == cluster_index) {
            
            if(partition_read_sector(&fs->partition, clus_sector + sector_off, segment, offset)) return 1;
            return 0;
            
        }
        
        if(fs->type == TYPE_FAT12) {
            
            uint16_t fat_offset = cluster + (cluster / 2);
            uint16_t fat_sector = fs->reserved_sectors + (fat_offset / 512);
            uint16_t ent_offset = fat_offset % 512;
            uint16_t val;
            
            if(partition_read_sector(&fs->partition, fat_sector, 0, (uint16_t)disk_tmp_buffer)) {
                
                return 1;
                
            }
            
            val = *(uint16_t*)&disk_tmp_buffer[ent_offset];
            if(cluster & 1) cluster = val >> 4;
            else cluster = val & 0x0fff;
            
            if(cluster >= 0xff7) {
                
                return 1;
                
            }
            
        } else if(fs->type == TYPE_FAT16) {
            
            uint16_t fat_offset = cluster * 2;
            uint16_t fat_sector = fs->reserved_sectors + (fat_offset / 512);
            uint16_t ent_offset = fat_offset % 512;
            
            if(partition_read_sector(&fs->partition, fat_sector, 0, (uint16_t)disk_tmp_buffer)) {
                
                return 1;
                
            }
            
            cluster = *(uint16_t*)&disk_tmp_buffer[ent_offset];
            
            if(cluster >= 0xfff7) {
                
                return 1;
                
            }
            
        } else return 1;
        
        cluster_i++;
        
    }
    
}

uint8_t fat12_16_read_file(fat12_16_t* fs, fat_dir_entry_t* file, uint16_t segment, uint16_t offset) {
    
    uint16_t cluster = file->cluster_low;
    
    while(1) {
        
        uint16_t clus_sector = ((cluster - 2) * fs->sectors_per_cluster) + fs->data_start;
        
        uint8_t i = 0;
        for(; i < fs->sectors_per_cluster; i++) {
            
            if(partition_read_sector(&fs->partition, clus_sector, segment, offset)) return 1;
            offset += 512;
            
        }
        
        if(fs->type == TYPE_FAT12) {
            
            uint16_t fat_offset = cluster + (cluster / 2);
            uint16_t fat_sector = fs->reserved_sectors + (fat_offset / 512);
            uint16_t ent_offset = fat_offset % 512;
            uint16_t val;
            
            if(partition_read_sector(&fs->partition, fat_sector, 0, (uint16_t)disk_tmp_buffer)) {
                
                return 1;
                
            }
            
            val = *(uint16_t*)&disk_tmp_buffer[ent_offset];
            if(cluster & 1) cluster = val >> 4;
            else cluster = val & 0x0fff;
            
            if(cluster >= 0xff7) {
                
                return 0;
                
            }
            
        } else if(fs->type == TYPE_FAT16) {
            
            uint16_t fat_offset = cluster * 2;
            uint16_t fat_sector = fs->reserved_sectors + (fat_offset / 512);
            uint16_t ent_offset = fat_offset % 512;
            
            if(partition_read_sector(&fs->partition, fat_sector, 0, (uint16_t)disk_tmp_buffer)) {
                
                return 1;
                
            }
            
            cluster = *(uint16_t*)&disk_tmp_buffer[ent_offset];
            
            if(cluster >= 0xff7) {
                
                return 0;
                
            }
            
        } else return 1;
        
    }
    
}

uint8_t fat12_16_read_file_bytes(fat12_16_t* fs, fat_dir_entry_t* file, uint16_t seek, uint16_t size, uint16_t segment, uint16_t offset) {
    
    size_t start = seek / 512;
    size_t start_off = seek % 512;
    size_t end_size = (seek + size) % 512;
    size_t end = (seek + size) / 512;
    
    size_t buf_off = 0;
    size_t i;
    size_t off;
    size_t _size;
    
    for(i = start; i <= end; i++) {
        
        if(fat12_16_read_file_sector(fs, file, i, 0, (size_t)disk_tmp_buffer)) return 1;
        
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
