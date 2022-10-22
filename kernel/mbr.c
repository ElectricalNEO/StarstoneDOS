#include "mbr.h"
#include "heap.h"
#include "memory.h"

partition_list_t* init_partitions(drive_list_t* drives) {
    
    partition_list_t* partitions_head = kmalloc(sizeof(partition_list_t));
    partition_list_t* partition_current = partitions_head;
    drive_list_t* drive;
    
    for(drive = drives; drive->next; drive = drive->next) {
        
        if(drive->data.drive_num < 0x80) {
            
            partition_current->data.bootable = 1;
            partition_current->data.drive = drive->data;
            partition_current->data.lba = 0;
            partition_current->data.part_num = 0;
            
            partition_current->next = kmalloc(sizeof(partition_list_t));
            partition_current = partition_current->next;
            continue;
            
        }
        
        if(!drive_read_sector_x(&drive->data, 0, 0, (uint16_t)disk_tmp_buffer, 5)) {
            
            uint8_t i = 0;
            for(; i < 4; i++) {
                
                if(((mbr_t*)disk_tmp_buffer)->partitions[i].system_id) {
                    
                    partition_current->data.bootable = ((mbr_t*)disk_tmp_buffer)->partitions[i].bootable;
                    partition_current->data.drive = drive->data;
                    partition_current->data.lba = ((mbr_t*)disk_tmp_buffer)->partitions[i].lba_start;
                    partition_current->data.part_num = i;
                    
                    partition_current->next = kmalloc(sizeof(partition_list_t));
                    partition_current = partition_current->next;
                    
                }
                
            }
            
        }
        
    }
    
    kfree(partition_current->next);
    partition_current->next = 0;
    
    return partitions_head;
    
}

uint8_t partition_read_sector(partition_t* partition, uint32_t lba, uint16_t segment, uint16_t offset) {
    
    return drive_read_sector_x(&partition->drive, lba + partition->lba, segment, offset, 5);
    
}
