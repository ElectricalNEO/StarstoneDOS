#include "stdio.h"
#include "drive.h"
#include "heap.h"
#include "mbr.h"
#include "fs.h"
#include "fat12_16.h"

extern char _BREAK;

void start(uint8_t drive) {
    
    fat12_16_t* boot_fs = 0;
    
    drive_list_t* drives;
    drive_list_t* drives_node;
    
    partition_list_t* partitions;
    partition_list_t* partitions_node;
    
    fat12_16_list_t* fats;
    fat12_16_list_t* fats_node;
    
    clear();
    set_attributes(0x0f);
    
    printf("Starstone 1.0\r\n");
    
    init_heap(&_BREAK);
    
    fats = kmalloc(sizeof(fat12_16_list_t));
    fats_node = fats;
    
    drives = init_drives();
    
    for(drives_node = drives; drives_node->next; drives_node = drives_node->next) {
        
        printf("Drive: %s%d\r\n", (drives_node->data.drive_num >= 0x80) ? "hd" : "fd", drives_node->data.drive_num % 0x80);
        
    }
    
    partitions = init_partitions(drives);
    fats = kmalloc(sizeof(fat12_16_list_t));
    
    for(partitions_node = partitions; partitions_node->next; partitions_node = partitions_node->next) {
        
        const char* fs;
        fat12_16_t fat;
        
        filesystem_type_t fs_type = detect_filesystem(&partitions_node->data);
        
        switch(fs_type) {
            
            case FAT12: fs = "FAT12"; break;
            case FAT16: fs = "FAT16"; break;
            case UNKNOWN: fs = "UNKNOWN"; break;
            
        }
        
        switch(fs_type) {
            
            case FAT12:
            case FAT16:
                if(init_fat12_16(&partitions_node->data, &fat)) {
                    
                    printf("WARNING: Failed to initialize file system on %s%dp%d", (partitions_node->data.drive.drive_num >= 0x80) ? "hd" : "fd", partitions_node->data.drive.drive_num % 0x80, partitions_node->data.part_num);
                    if(partitions_node->data.drive.drive_num == drive && &partitions_node->data.bootable) {
                        
                        printf("ERROR: Failed to initialize file system on the boot partition!\r\nHalting...\r\n");
                        
                    }
                    
                } else {
                    
                    if(partitions_node->data.drive.drive_num == drive && partitions_node->data.bootable) {
                        
                        boot_fs = &fats->data;
                        
                    }
                    
                    fats->data = fat;
                    fats->next = kmalloc(sizeof(fat12_16_list_t));
                    fats = fats->next;
                    
                }
                break;
            default:
                fs = "UNKNOWN";
        }
        
        printf("Partition: %s%dp%d (LBA: %D; FS: %s)\r\n", (partitions_node->data.drive.drive_num >= 0x80) ? "hd" : "fd", partitions_node->data.drive.drive_num % 0x80, partitions_node->data.part_num, partitions_node->data.lba, fs);
        
    }
    
    fats->next = 0;
    
    if(!boot_fs) {
        
        printf("ERROR: Failed to initialize file system on the boot partition!\r\nHalting...\r\n");
        while(1);
        
    }
    
    puts("Initialization complete.\r\n");
    
    {
        
        char* str = "1234567";
        fat_dir_entry_t loremips = fat12_16_find_in_root_dir(boot_fs, "LOREMIPSTXT");
        if(!loremips.cluster_low) {
            
            printf("ERROR: Failed to open file \"LOREMIPS.TXT\"!\r\nHalting...\r\n");
            while(1);
            
        }
        
        fat12_16_read_file_bytes(boot_fs, &loremips, 2, 7, 0, (uint16_t)str);
        
        printf(str);
        
    }
    
    while(1);
    
}
