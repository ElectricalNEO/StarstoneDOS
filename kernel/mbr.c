#include "mbr.h"
#include "heap.h"
#include "memory.h"

partition_t* init_partitions(drive_t* drives, uint8_t drive_count, uint16_t* partition_count) {
    
    partition_t* partitions;
    uint8_t i, j, k;
    *partition_count = 0;
    
    for(i = 0; i < drive_count; i++) {
        
        if(drives[i].drive_num < 0x80) {
            
            (*partition_count)++;
            continue;
            
        }
        
        if(drive_read_sector_x(&drives[i], 0, 0, (uint16_t)disk_tmp_buffer, 5)) {
            
            continue;
            
        }
        
        for(j = 0; j < 4; j++) if(((mbr_t*)disk_tmp_buffer)->partitions[j].system_id) (*partition_count)++;
        
    }
    
    if(*partition_count == 0) return 0;
    
    partitions = kmalloc(*partition_count * sizeof(partition_t));
    
    k = 0;
    
    for(i = 0; i < drive_count; i++) {
        
        if(drives[i].drive_num < 0x80) {
            
            partitions[k].drive = drives[i];
            partitions[k].lba = 0;
            partitions[k].part_num = 0;
            k++;
            continue;
            
        }
        
        if(drive_read_sector_x(&drives[i], 0, 0, (uint16_t)disk_tmp_buffer, 5)) {
            
            continue;
            
        }
        
        for(j = 0; j < 4; j++) if(((mbr_t*)disk_tmp_buffer)->partitions[j].system_id) {
            
            partitions[k].drive = drives[i];
            partitions[k].lba = ((mbr_t*)disk_tmp_buffer)->partitions[j].lba_start;
            partitions[k].part_num = j;
            k++;
            
        }
        
    }
    
    return partitions;
    
}

uint8_t partition_read_sector(partition_t* partition, uint32_t lba, uint16_t segment, uint16_t offset) {
    
    return drive_read_sector_x(&partition->drive, lba + partition->lba, segment, offset, 5);
    
}
