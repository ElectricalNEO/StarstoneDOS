#include "stdio.h"
#include "drive.h"
#include "heap.h"
#include "mbr.h"
#include "fs.h"
#include "fat12_16.h"

extern char _BREAK;

void start(uint8_t drive) {
    
    uint8_t drives_connected;
    drive_t* drives;
    partition_t* partitions;
    fat12_16_t* fats;
    drive_t* boot_disk = 0;
    fat12_16_t* boot_fs = 0;
    uint8_t i = 0;
    fat_dir_entry_t loremips;
    uint8_t* bytes;
    
    uint16_t partition_count;
    
    clear();
    set_attributes(0x0f);
    
    puts("Starstone 1.0\r\n");
    
    init_heap(&_BREAK);
    
    bytes = (uint8_t*)kmalloc(8);
    
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
    fats = kmalloc(sizeof(fat12_16_t) * partition_count);
    
    printf("Partition count: %d\r\n", partition_count);
    
    for(i = 0; i < partition_count; i++) {
        
        const char* fs;
        
        switch(detect_filesystem(&partitions[i])) {
            
            case FAT12:
                fs = "FAT12";
                if(init_fat12_16(&partitions[i], &fats[i])) {
                    printf("WARNING: Failed to initialize FAT12 on %s%dp%d", (partitions[i].drive.drive_num >= 0x80) ? "hd" : "fd", partitions[i].drive.drive_num % 0x80, partitions[i].part_num);
                    if(partitions[i].drive.drive_num == drive && partitions[i].bootable) {
                        
                        printf("ERROR: Failed to initialize file system on the boot partition!\r\nHalting...\r\n");
                        
                    }
                }
                break;
            case FAT16:
                fs = "FAT16";
                if(init_fat12_16(&partitions[i], &fats[i])) {
                    printf("WARNING: Failed to initialize FAT16 on %s%dp%d", (partitions[i].drive.drive_num >= 0x80) ? "hd" : "fd", partitions[i].drive.drive_num % 0x80, partitions[i].part_num);
                    if(partitions[i].drive.drive_num == drive && partitions[i].bootable) {
                        
                        printf("ERROR: Failed to initialize file system on the boot partition!\r\nHalting...\r\n");
                        
                    }
                }
                break;
            default:
                fs = "UNKNOWN";
        }
        
        if(partitions[i].drive.drive_num == drive && partitions[i].bootable) {
            
            boot_fs = &fats[i];
            
        }
        
        printf("Partition: %s%dp%d (LBA: %D; FS: %s)\r\n", (partitions[i].drive.drive_num >= 0x80) ? "hd" : "fd", partitions[i].drive.drive_num % 0x80, partitions[i].part_num, partitions[i].lba, fs);
        
    }
    
    if(!boot_fs) {
        
        printf("ERROR: Failed to initialize file system on the boot partition!\r\nHalting...\r\n");
        
    }
    
    puts("Initialization complete.\r\n");
    
    loremips = fat12_16_find_in_root_dir(boot_fs, "LOREMIPSTXT");
    
    if(loremips.cluster_low) {
        
        printf("File \"LOREMIPS.TXT\" found!\r\n");
        
    } else {
        
        printf("File \"LOREMIPS.TXT\" not found!\r\nHalting...\r\n");
        while(1);
        
    }
    
    fat12_16_read_file_bytes(boot_fs, &loremips, 0x1ff, 7, 0, (uint16_t)bytes);
    bytes[7] = 0;
    
    printf("Done!\r\n");
    
    printf(bytes);
    
    while(1);
    
}
