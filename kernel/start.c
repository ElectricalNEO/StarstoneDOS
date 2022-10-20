#include "stdio.h"
#include "drive.h"
#include "heap.h"
#include "mbr.h"
#include "fs.h"
#include "itoa.h"
#include "divmul32.h"

drive_t* boot_disk = 0;

extern char _BREAK;

void start(uint8_t drive) {
    
    uint8_t i;
    uint8_t drives_connected;
    drive_t* drives;
    partition_t* partitions;
    uint16_t partition_count;
    
    clear();
    set_attributes(0x0f);
    
    puts("Starstone 1.0\r\n");
    
    init_heap(&_BREAK);
    
    drives = init_drives(&drives_connected);
    
    for(i = 0; i < drives_connected; i++) {
        
        printf("Drive: %s%d\r\n", (drives[i].drive_num >= 0x80) ? "hd" : "fd", drives[i].drive_num % 0x80);
        if(drives[i].drive_num == drive) boot_disk = &drives[i];
        
    }
    
    if(!boot_disk) {
        
        printf("Failed to get boot disk's (0x%x) parameters!\r\nHalting...\r\n", drive);
        while(1);
        
    }
    
    partitions = init_partitions(drives, drives_connected, &partition_count);
    
    printf("Partition count: %d\r\n", partition_count);
    
    for(i = 0; i < partition_count; i++) {
        
        const char* fs = "E";
        
        switch(detect_filesystem(&partitions[i])) {
            
            case FAT12:
                fs = "FAT12";
                break;
            case FAT16:
                fs = "FAT16";
                break;
            default:
                fs = "UNKNOWN";
            
        }
        
        printf("Partition: %s%dp%d (LBA: %D; FS: %s)\r\n", (partitions[i].drive.drive_num >= 0x80) ? "hd" : "fd", partitions[i].drive.drive_num % 0x80, partitions[i].part_num, partitions[i].lba, fs);
        
    }
    
    puts("Initialization complete.\r\n");
    
    while(1);
    
}
