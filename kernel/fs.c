#include "fs.h"
#include "divmul32.h"

filesystem_type_t detect_filesystem(partition_t* partition) {
    
    if(partition_read_sector(partition, 0, 0, (uint16_t)disk_tmp_buffer)) return UNKNOWN;
    
    if(((fat12_16_bpb_t*)disk_tmp_buffer)->signature == 0x28 || ((fat12_16_bpb_t*)disk_tmp_buffer)->signature == 0x29) {
        
        uint32_t total_sectors = ((fat12_16_bpb_t*)disk_tmp_buffer)->total_sectors == 0 ? ((fat12_16_bpb_t*)disk_tmp_buffer)->large_sector_count : ((fat12_16_bpb_t*)disk_tmp_buffer)->total_sectors;
        uint32_t root_dir_sectors = ((((fat12_16_bpb_t*)disk_tmp_buffer)->root_dir_entries * 32) + 511) / 512;
        uint32_t data_sectors = total_sectors - (((fat12_16_bpb_t*)disk_tmp_buffer)->reserved_sectors + (((fat12_16_bpb_t*)disk_tmp_buffer)->fats * ((fat12_16_bpb_t*)disk_tmp_buffer)->sectors_per_fat) + root_dir_sectors);
        uint16_t total_clusters = divide32(data_sectors, ((fat12_16_bpb_t*)disk_tmp_buffer)->sectors_per_cluster);
        
        if(total_clusters < 4085) return FAT12;
        return FAT16;
        
    }
    
    if(partition_read_sector(partition, 2, 0, (uint16_t)disk_tmp_buffer)) return UNKNOWN;
    
    if(((ext2_superblock_t*)disk_tmp_buffer)->signature == EXT2_SIGNATURE) return EXT2;
    
    return UNKNOWN;
    
}

uint8_t init_fs(fs_t* fs, partition_t* partition) {
    
    fs->type = UNKNOWN;
    
    switch(detect_filesystem(partition)) {
        
        case FAT12:
        case FAT16:
            return init_fat12_16(partition, fs);
        case EXT2:
            return init_ext2(partition, fs);
        default:
            return 1;
        
    }
    
}
